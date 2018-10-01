# The mutable Package

This package provides an implementation of mutable vectors in R as a
simple illustration of the `ALTREP` framework.

A simple example:
```r
x <- mutable("double", 2)
x
## [1] 0 0
y <- x
x[1] <- 1
y
## [1] 1 0
```

Only the data part of the vector is mutable; attribute assignments
obey standard R semantics:

```r
attr(x, "foo") <- 1
attributes(y)
## NULL
```

## Some Notes

- For now, only `integer` and `double` vectors are supported. Other
  atomic types shoud be easy to add. Support for `character` vectors
  is also probably straight forward. Vectors of type `list` may be
  more challenging as cycles need to be prevented.

- At the `C` level both deep and shallow duplicating preserves data
  sharing. Internally the object uses a separate R vector for the
  shared storage, but this vector should not be accessed outside the
  package `C` code.

- There is no special serialization support: A mutable vector is
  serialized as a standard unmutable vector. Sharing could be
  preserved within a serialization by using an external pointer as an
  interbediary.  It's not clear this would be worth while.

- For now, mutable vectors are marked internally with
  `MARK_NOT_MUTABLE` so optimizations, in particular in arithmetic
  operations, that re-use unreferenced vectors don't re-use a mutable
  vector. This means that attribute modifications may incur some
  additional overhead. This could be avoided if the internal R re-use
  code makes sure it was only re-using standard vectors. Of course
  re-using a private memory-mapped vector might be OK, so maybe a
  method to query whethr re-use is OK would be helpful.

 - There is no support for growing or shrinking a mutable vector.
   Support along the lines of Common Lisp fill pointers could be
   added. It's not clear whether integrating this with the growable
   vector concept used when assigning beyond the current length of a
   vector sould be a good idea.

- Possible additions (can be done at the R level):
  - Allow specifying initial data in the `mutable()` call.
  - Add a `freeze` or `unmutable` function.
