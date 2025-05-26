input_data_1 = {
    "rows": [
        ("A", 2)     # row "A" has 2 data (eg. ab,bc)
    ],

    "cols": [
        ("X", 2)     # column "X" has 2 data (eg. ab,bc)
    ],

    "target": {
        ("A"): 4    # target value 4 corresponding to row "A"
    },
    
    "default":{
        ("A", "X"): 0
    },

    "pairings": {
        # key: (row, col, pattern of appropriate length)
        # pattern: tuple of int or '*' (wildcard)

        ("A", "X", (1, 2))      : 5,            # (a, b) --> (a, b) then 5
        ("A", "X", (1, '*'))    : 2,            # (a, b) --> (a, *) then 2
        ("A", "X", (2, 1))      : 3,            # (a, b) --> (b, a) then 3
        ("A", "X", ('*', 2))    : 1,            # (a, b) --> (*, b) then 1
        ("A", "X", ('*', '*'))  : 8,            # (a, b) --> (*, *) then 8
        
        # If any other pairing then default value : 0

    }
}

input_data_2 = {
    "rows": [
        ("A", 2),
        ("B", 1)
    ],

    "cols": [
        ("X", 1),
        ("Y", 2)
    ],

    "target": {
        ("A"): 4,
        ("B"): 7
    },
    
    "default": {
        ("A", "X"): 0,
        ("A", "Y"): 0,
        ("B", "X"): 0,
        ("B", "Y"): 0
    },

    "pairings": {
        # A -> X
        ("A", "X", (1))     : 6,        # (a, b) --> (a) then 6
        ("A", "X", (2))     : 3,        # (a, b) --> (b) then 3
        ("A", "X", ('*'))   : 1,        # (a, b) --> (*) then 1

        # A -> Y
        ("A", "Y", (1, 2))      : 4,    # (a, b) --> (a, b) then 4
        ("A", "Y", ('*', 2))    : 2,    # (a, b) --> (*, b) then 2
        ("A", "Y", ('*', '*'))  : 0,    # (a, b) --> (*, *) then 0

        # B -> X
        ("B", "X", (1))      : 7,       # (a) --> (a) then 7
        ("B", "X", ('*'))    : 3,       # (a) --> (*) then 3

        # B -> Y
        ("B", "Y", (1, '*'))    : 5,    # (a) --> (a, *) then 5
        ("B", "Y", ('*', 1))    : 2,    # (a) --> (*, a) then 2
        ("B", "Y", ('*', '*'))  : 0,    # (a) --> (*, *) then 0
    }
}
