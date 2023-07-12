
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

## Example usage using standalone dataset

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

## Example usage using OMOP CDM / PLP package

``` r
# remotes::install_github("OHDSI/PatientLevelPrediction@explore")
library(PatientLevelPrediction)
library(Eunomia)
#> Loading required package: DatabaseConnector
library(Explore)

library(reshape2) # needed for PLP?
```

Load data:

``` r
connectionDetails <- Eunomia::getEunomiaConnectionDetails()
Eunomia::createCohorts(connectionDetails)

covSet <- FeatureExtraction::createCovariateSettings(useDemographicsGender = T,
                                                     useDemographicsAge = T,
                                                     useDemographicsRace = T,
                                                     useDemographicsEthnicity = T,
                                                     useDemographicsAgeGroup = T,
                                                     useConditionGroupEraLongTerm = T,
                                                     useDrugEraStartLongTerm  = T,
                                                     endDays = -1)

databaseDetails <- PatientLevelPrediction::createDatabaseDetails(
  connectionDetails = connectionDetails,
  cdmDatabaseSchema = "main",
  cohortDatabaseSchema = "main",
  cohortTable = "cohort",
  targetId = 4,
  outcomeIds = 3,
  outcomeDatabaseSchema = "main",
  outcomeTable =  "cohort",
  cdmDatabaseName = 'eunomia'
)

restrictPlpDataSettings <- PatientLevelPrediction::createRestrictPlpDataSettings(
  firstExposureOnly = T,
  washoutPeriod = 365
)

plpData <- PatientLevelPrediction::getPlpData(
  databaseDetails = databaseDetails,
  restrictPlpDataSettings = restrictPlpDataSettings,
  covariateSettings = covSet
)

populationSettings <- PatientLevelPrediction::createStudyPopulationSettings(
  requireTimeAtRisk = F,
  riskWindowStart = 1,
  riskWindowEnd = 365)
```

Fit model with defaults and/or input parameters:

``` r
modelSettings <- PatientLevelPrediction::setExplore(aucCurve = FALSE, sort_by = "random")

plpResults <- PatientLevelPrediction::runPlp( 
  plpData = plpData, 
  outcomeId = 3,
  analysisId = 'demo', 
  analysisName = 'run plp demo', 
  populationSettings = populationSettings, 
  splitSettings = createDefaultSplitSetting(
    type = "time",
    testFraction = 0.25,
    nfold = 2
  ), 
  sampleSettings = createSampleSettings(),
  preprocessSettings = createPreprocessSettings(
    minFraction = 0, 
    normalize = T
  ), 
  modelSettings = modelSettings, 
  executeSettings = createDefaultExecuteSettings(), 
  saveDirectory = "./plpdemo"
)
```

Predict:

``` r
prediction <- plpResults$prediction$value
```

Additional documentation includes: - Vignette code examples in
combination with PLP: [to be
added](~/Documents/Git/Explore/vignettes/EXPLORE_withPLP.Rmd) - Package
manual: [to be added](~/Documents/Git/Explore/vignettes/Explore_1.0.pdf)

## Development status

EXPLORE is under active development.

## Publication

Rijnbeek, P.R., Kors, J.A. Finding a short and accurate decision rule in
disjunctive normal form by exhaustive search. Machine Learning 80, 33–62
(2010). <https://doi.org/10.1007/s10994-010-5168-9>
