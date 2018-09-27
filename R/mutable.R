# Copyright 2018 R Core Team
# Distributed under GPL 2 or 3

mutable <- function(type = "double", length = 0L) {
    if (length > 0)
        .Call(C_make_mutable, vector(type, length))
    else
        vector(type, 0L)
}

is.mutable <- function(x)
    .Call(C_is_mutable, x)
