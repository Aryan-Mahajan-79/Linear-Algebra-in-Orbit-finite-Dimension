def check_input_validity(input_data):
    if not validate_orbit_names(input_data):
        return False

def validate_orbit_names(input_data):
    """
    Validate that:
    - No duplicate names in 'rows' or 'cols'.
    - No name appears in both 'rows' and 'cols'.

    Args:
        input_data (dict): Contains 'rows' and 'cols' as lists of (name, count) tuples.

    Returns:
        bool: True if names are unique, False otherwise.
    """
    rows = input_data.get("rows", [])
    cols = input_data.get("cols", [])

    row_names = [name for name, _ in rows]
    col_names = [name for name, _ in cols]
    names = names = row_names + col_names

    duplicate_name = len(set(names)) != len(names)

    if duplicate_name:
        print("❌ Duplicate names found:", names)

    return not (duplicate_name)
