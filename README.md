

# Explore <a href="https://mi-erasmusmc.github.io/Explore/"><img src="man/logo.png" align="right" height="220" alt="" /></a>

<!-- badges: start -->
[![R-CMD-check](https://github.com/mi-erasmusmc/Explore/actions/workflows/check-standard.yaml/badge.svg)](https://github.com/mi-erasmusmc/Explore/actions/workflows/check-standard.yaml)
[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://lifecycle.r-lib.org/articles/stages.html#experimental)
[![Codecov test
coverage](https://codecov.io/github/mi-erasmusmc/Explore/branch/develop/graph/badge.svg)](https://app.codecov.io/github/mi-erasmusmc/Explore?branch=develop)
<!-- badges: end -->

## Package overview

Functions to find a short and accurate decision rule in disjunctive
normal form using the Exhaustive Procedure for LOgic-Rule Extraction
(EXPLORE) algorithm. The application performs and exhaustive search on
all Boolean Normal Form decision rules.

## Package installation

You can install the latest version of EXPLORE like so:

``` r
install.packages("remotes")
remotes::install_github("mi-erasmusmc/EXPLORE")
```

Additional instructions: to be added.

## Example usage using iris dataset

``` r
library(Explore)
library(farff)
```

Load data:

``` r
data <- farff::readARFF(system.file("examples/iris.arff", package = "Explore"))
output_path <- file.path(getwd(), "output//")
```

Fit model with defaults and/or input parameters:

``` r
model <- Explore::trainExplore(output_path = output_path, 
                               file_name = "iris", 
                               train_data = data, 
                               ClassFeature = "'class'", 
                               PositiveClass = '"Iris-versicolor"')
```

Predict:

``` r
prediction <- Explore::predictExplore(model, test_data = data)
```

## Development status

EXPLORE is under active development.

## Publication

Rijnbeek, P.R., Kors, J.A. Finding a short and accurate decision rule in
disjunctive normal form by exhaustive search. Machine Learning 80, 33–62
(2010). <https://doi.org/10.1007/s10994-010-5168-9>
