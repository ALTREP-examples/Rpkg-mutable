% Copyright 2018 R Core Team
% Distributed under GPL 2 or 3

\name{mutable}
\alias{mutable}
\alias{is.mutable}
\title{Mutable Vectors}
\description{
  Create and test for mutable vectors.
}
\usage{
mutable(type = "double", length = 0L)
is.mutable(x)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{type}{character string naming the atomic type.}
  \item{length}{non-negative integer specifying the desired length.}
  \item{x}{an R object.}
}
\details{
  Mutable vectors allow pass by reference semantics. THey should be used
  with care.
}
\value{
  \code{mutable} returns a vector of the specified type with mutable
  data. Numeric vector elements are initialized to zero.

  \code{is.mutable} returns \code{TRUE} if its argument is a mutable
  vector and \code{FALSE} if it is not.
}
%%\references{
%% ~put references to the literature/web site here ~
%%}
\author{
  Luke Tierney
}

\examples{
x <- mutable("double", 5)
x[] <- 1:5
y <- x
x[1] <- 6
y[1]
is.mutable(x)
is.mutable(y)
is.mutable(1:5)
}
\keyword{file}
