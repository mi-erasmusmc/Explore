library(Explore)

### Tests for EXPLORE using  iris dataset
data <- farff::readARFF("data/iris.arff")
output_path <- paste0(getwd(), "/tests/")

### Train
# Option 0: fully pre-specified settings file
Explore::runExplore(paste0(output_path, "iris.project"))

# Option 1: fully pre-specified settings file
model <- Explore::trainExplore(output_path = output_path, file_name = "iris", settings_path = paste0(output_path, "iris.project"))

# Option 2: pre-specified settings file with input data
model <- Explore::trainExplore(output_path = output_path, train_data = data, settings_path = paste0(output_path, "train_data.project"), ClassFeature = "'class'", PositiveClass = '"Iris-versicolor"',  PrintSettings = "no", PrintPerformance = "no")

# Option 3: input parameters (no settings file) 
model <- Explore::trainExplore(output_path = output_path, train_data = data, ClassFeature = "'class'", PositiveClass = '"Iris-versicolor"', FeatureInclude = "'petallength'")

###  Predict
prediction <- Explore::predictExplore(output_path, model, test_data = data)