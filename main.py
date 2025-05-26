from input_example import *
from input_validity import check_input_validity

if __name__ == "__main__":
    input_data = input_data_1
    if not check_input_validity(input_data):
        exit(1)