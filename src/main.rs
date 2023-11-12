use std::fs::{self, DirEntry};
use std::path::Path;
use std::io;
use regex::Regex;
use glob::glob;

fn main() -> io::Result<()> {
    let mut path = String::new();
    println!("Enter the path to the folder with the images: ");
    io::stdin().read_line(&mut path)?;
    path = path.trim().to_string();

    organize_images(&path)?;
    Ok(())
}

fn organize_images(path: &str) -> io::Result<()> {
    for entry in fs::read_dir(path)? {
        let entry = entry?;
        let path = entry.path();
        if path.is_file() {
            if let Some(year) = extract_year_from_filename(&entry) {
                let year_dir = format!("{}/{}", path.parent().unwrap().to_str().unwrap(), year);
                fs::create_dir_all(&year_dir)?;
                fs::rename(&path, Path::new(&year_dir).join(entry.file_name()))?;
            }
        }
    }
    Ok(())
}

fn extract_year_from_filename(entry: &DirEntry) -> Option<String> {
    let filename = entry.file_name().to_string_lossy().into_owned();

    let patterns = vec![
        Regex::new(r"IMG-(\d{4})\d{4}-WA\d{4}").unwrap(), // IMG-YYYYMMDD-WAXXXX
        Regex::new(r"(\d{4})\d{4}_\d{6}").unwrap(),      // YYYYMMDD_XXXXXX
    ];

    for pattern in patterns {
        if let Some(caps) = pattern.captures(&filename) {
            return Some(caps[1].to_string());
        }
    }

    None
}

// Unit tests
fn count_test_files(directory: &str, pattern: &str) -> io::Result<usize> {
    let pattern = format!("{}/{}", directory, pattern);
    Ok(glob(&pattern)
        .expect("Failed to read glob pattern")
        .filter_map(Result::ok)
        .count())
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::ffi::OsString;
    use std::fs::File;
    use std::io::{self, BufRead};

    #[test]
    fn test_extract_year_from_multiple_files() -> io::Result<()> {
        let test_file_count = count_test_files("tests", "test*.txt")?;
        let reference_file_count = count_test_files("tests", "reference*.txt")?;
        assert_eq!(test_file_count, reference_file_count, "Mismatch in the number of test and reference files");

        for i in 1..=test_file_count {
            let test_filename = format!("tests/test{}.txt", i);
            let reference_filename = format!("tests/reference{}.txt", i);

            let test_cases = read_lines(&test_filename)?;
            let expected_results = read_lines(&reference_filename)?;

            for (test_case, expected) in test_cases.iter().zip(expected_results.iter()) {
                let mock_entry = MockDirEntry::new(test_case);
                let result = extract_year_from_filename(&mock_entry);
                let expected_option = if expected == "None" { None } else { Some(expected.clone()) };
                assert_eq!(result, expected_option);
            }
        }
        Ok(())
    }

    // Mock DirEntry implementation for testing
    struct MockDirEntry {
        file_name: OsString,
    }

    impl MockDirEntry {
        fn new(file_name: &str) -> MockDirEntry {
            MockDirEntry {
                file_name: OsString::from(file_name),
            }
        }
    }

    impl std::ops::Deref for MockDirEntry {
        type Target = DirEntry;

        fn deref(&self) -> &Self::Target {
            panic!("Not implemented, only for testing file_name")
        }
    }

    // Helper function to read lines from a file
    fn read_lines<P: AsRef<Path>>(filename: P) -> io::Result<Vec<String>> {
        let file = File::open(filename)?;
        let lines = io::BufReader::new(file).lines();
        lines.collect()
    }
}