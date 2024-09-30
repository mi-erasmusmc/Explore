testExplore <- function(dataset = "iris", StartRulelength = 2, EndRulelength = 2, BinaryReduction = FALSE) {
  # dataset = "iris"
  # dataset = "binary_3"
  # dataset = "binary_10"
  # dataset = "continuous_4"
  # dataset = "categorical_4"
  # dataset = "mix_4"
  # StartRulelength = 2
  # EndRulelength = 3
  # BinaryReduction = FALSE
  # BinaryReduction = TRUE
  
  config <- getDataSetPath(dataset = dataset) 
  
  ### Tests for EXPLORE using iris dataset
  train_data <- farff::readARFF(config$data_path)
  output_path <- paste0(tempdir(), "/", glue::glue("{getRandomId()}"), "/")
  file_name <- paste0(dataset, "_train_data")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  
  result <- trainExplore(train_data = train_data,
                         settings_path = NULL,
                         output_path = output_path,
                         file_name = file_name,
                         OutputFile = NULL,
                         StartRulelength = StartRulelength,
                         EndRulelength = EndRulelength,
                         OperatorMethod = "EXHAUSTIVE",
                         CutoffMethod = "RVAC",
                         ClassFeature = config$class_feature,
                         PositiveClass = config$positive_class,
                         FeatureInclude = "",
                         Maximize = "ACCURACY",
                         Accuracy = 0,
                         BalancedAccuracy = 0,
                         Specificity = 0,
                         PrintSettings = TRUE,
                         PrintPerformance = TRUE,
                         Subsumption = TRUE,
                         BranchBound = TRUE,
                         Parallel = FALSE,
                         PrintCutoffSets = TRUE,
                         Sorted = "none",
                         OutputMethod = "EVERY",
                         BinaryReduction = BinaryReduction)
  
  unlink(output_path, recursive = TRUE)
  return(result)
  
}

getDataSetPath <- function(dataset = "iris") {
  
  options <- c("iris",
               "binary_3", 
               "binary_10",
               "continuous_4_small",
               "continuous_4",
               "categorical_4_large", 
               "categorical_4", 
               "mix_4", 
               "mix_4_ordered")
  if (!dataset %in% options) {
    stop(glue::glue("Wrong dataset name, {dataset} is not one of {options}"))
  }
  
  # Assing paths
  if (dataset == "iris") {
    data_path <- system.file("examples", "tests", "iris.arff", package = "Explore")
    settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  } else {
    fileNamePath <- paste0(dataset, ".arff")
    data_path <- system.file("examples", "complexity", fileNamePath, package = "Explore")
    # fileNameSettingsPath <- paste0(dataset, ".project")
    # settings_path <- system.file("examples", "complexity", fileNameSettingsPath, package = "Explore")
  }
  
  # Assign class
  if (dataset %in% c("binary_10",
                     "binary_3",
                     "mix_4", 
                     "mix_4_ordered")) {
    class_feature <- "'outcomeCount'"
    positive_class <- '"1"'
  } else if (dataset == "iris" | dataset == "continuous_4_small" | dataset == "continuous_4" | dataset == "categorical_4_large" | dataset == "categorical_4") {
    class_feature <- "'class'"
    positive_class <- '"Iris-versicolor"'
  } 
  checkmate::assert_file_exists(data_path)
  # checkmate::assert_file_exists(settings_path)
  checkmate::assert_character(class_feature)
  
  result <- list("data_path" = data_path,
                 # "settings_path" = settings_path,
                 "class_feature" = class_feature,
                 "positive_class" = positive_class)
  
  return(result)
  
}

getRandomId <- function() {
  chars <- c(0:9, letters, LETTERS)
  randomId <- stringr::str_c(sample(chars, 4, replace = TRUE) , collapse = "")
  return(randomId)
}