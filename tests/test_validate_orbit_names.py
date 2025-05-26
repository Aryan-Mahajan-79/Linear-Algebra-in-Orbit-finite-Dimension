import pytest
from input_validity import validate_orbit_names


def test_no_duplicates_no_overlap():
    input_data = {
        "rows": [("A", 2), ("B", 1)],
        "cols": [("X", 2), ("Y", 3)]
    }
    assert validate_orbit_names(input_data) is True

def test_duplicate_row_names():
    input_data = {
        "rows": [("A", 2), ("A", 1)],  # duplicate 'A'
        "cols": [("X", 2), ("Y", 3)]
    }
    assert validate_orbit_names(input_data) is False

def test_duplicate_col_names():
    input_data = {
        "rows": [("A", 2), ("B", 1)],
        "cols": [("X", 2), ("X", 3)]  # duplicate 'X'
    }
    assert validate_orbit_names(input_data) is False

def test_overlap_names():
    input_data = {
        "rows": [("A", 2), ("B", 1)],
        "cols": [("B", 2), ("Y", 3)]  # 'B' appears in both rows and cols
    }
    assert validate_orbit_names(input_data) is False