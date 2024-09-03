testExplore <- function(dataset = "iris", StartRulelength = 2, EndRulelength = 2, BinaryReduction = FALSE) {
  # dataset = "iris"
  # dataset = "binary_3"
  # dataset = "binary_10"
  # dataset = "continuous_4"
  # dataset = "categorical_4"
  # dataset = "mix_4"
  # StartRulelength = 1
  # EndRulelength = 1
  # BinaryReduction = FALSE
  # BinaryReduction = TRUE
  
  config <- getDataSetPath(dataset = dataset) 
  
  ### Tests for EXPLORE using iris dataset
  train_data <- farff::readARFF(config$data_path)
  # settings <- paste0(paste(readLines(config$settings_path), collapse="\n"),"\n")
  output_path <- paste0(tempdir(), "/", glue::glue("{getRandomId()}"), "/")
  file_name <- paste0(dataset, "_train_data")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  
  train_data <- train_data
  settings_path <- NULL
  output_path <- output_path 
  file_name <- file_name
  OutputFile <- NULL
  StartRulelength <- StartRulelength
  EndRulelength <- EndRulelength
  OperatorMethod <- "EXHAUSTIVE"
  CutoffMethod <- "RVAC"
  ClassFeature <- config$class_feature
  PositiveClass <- config$positive_class
  FeatureInclude <- ""
  Maximize <- "ACCURACY"
  Accuracy <- 0
  BalancedAccuracy <- 0
  Specificity <- 0
  PrintSettings <- TRUE
  PrintPerformance <- TRUE
  PrintCutoffSets <- FALSE
  Subsumption <- FALSE
  BranchBound <- FALSE
  Sorted <- "none"
  Parallel <- FALSE
  OutputMethod <- "EVERY"
  BinaryReduction <- BinaryReduction
  
  
  if (!dir.exists(output_path)) {
    dir.create(output_path)
  }
  
  # Create output folder
  if(!endsWith(output_path, "/")) {
    warning("Output path should end with /, add this")
    output_path <- paste0(output_path, "/")
  }
  
  # Variable checks
  errorMessage <- makeAssertCollection()
  
  # check output file
  if(is.null(OutputFile)) {
    OutputFile <- paste0(output_path, file_name, ".result")
  } else {
    checkmate::checkFileExists(OutputFile,
                               add = errorMessage)
  }
  
  # check settings_path
  if (!is.null(settings_path)) {
    checkmate::assertFileExists(
      settings_path,
      add = errorMessage
    )
  }
  
  # check vars
  checkmate::assert(checkClass(train_data, "data.frame"),
                    checkPathForOutput(output_path, overwrite = TRUE),
                    checkDouble(StartRulelength),
                    checkDouble(EndRulelength),
                    checkString(CutoffMethod),
                    checkString(OperatorMethod),
                    checkString(ClassFeature),
                    checkString(PositiveClass),
                    checkString(FeatureInclude),
                    checkString(Maximize),
                    checkString(OutputMethod),
                    checkDouble(Accuracy),
                    checkDouble(BalancedAccuracy),
                    checkDouble(Specificity),
                    checkLogical(PrintSettings),
                    checkLogical(PrintPerformance),
                    checkLogical(PrintCutoffSets),
                    checkLogical(Subsumption),
                    checkLogical(BranchBound),
                    checkLogical(BinaryReduction),
                    checkString(Sorted),
                    checkLogical(Parallel),
                    add = errorMessage,
                    combine = "and"
  )
  checkmate::reportAssertions(collection = errorMessage)
  
  PrintSettings <- ifelse(PrintSettings == TRUE, "yes", "no")
  PrintPerformance <- ifelse(PrintPerformance == TRUE, "yes", "no")
  PrintCutoffSets <- ifelse(PrintCutoffSets == TRUE, "yes", "no")
  Subsumption <- ifelse(Subsumption == TRUE, "yes", "no")
  BranchBound <- ifelse(BranchBound == TRUE, "yes", "no")
  Parallel <- ifelse(Parallel == TRUE, "yes", "no")
  Accuracy <- ifelse(Accuracy == 0, "", Accuracy)
  BalancedAccuracy <- ifelse(BalancedAccuracy == 0, "", BalancedAccuracy)
  Specificity <- ifelse(Specificity == 0, "", Specificity)
  BinaryReduction <- ifelse(BinaryReduction == TRUE, "yes", "no")
  
  # Create project setting
  if (is.null(settings_path)) {
    if (is.null(train_data)) {
      stop("Data is NULL, but is required when settings_path is not entered.")
    } else {
      # Load template
      settings <- paste0(paste(readLines(paste0(system.file(package="Explore"),"/settings/template.project")), collapse="\n"),"\n")
    }
  } else {
    # Load settings
    settings <- paste0(paste(readLines(settings_path), collapse="\n"),"\n")
  }
  
  # TODO: check train_data and correct if necessary?
  # drop nearly constant or extremely sparse values
  
  # Save train_data if entered
  if (!is.null(train_data)) {
    
    # Sort features in data if needed
    if (Sorted != "none") {
      
      # TODO: test these options
      ClassFeature_ <- stringr::str_remove_all(ClassFeature, "'")
      PositiveClass_ <- stringr::str_remove_all(PositiveClass, '"')
      if (Sorted == "pearson" || Sorted == "spearman" || Sorted == "kendall") {
        cor <- sapply(train_data[, -which(names(train_data) == ClassFeature_)], function(col) cor(col, train_data[ClassFeature_]==PositiveClass_, method=Sorted))
      } else if (Sorted == "jaccard") {
        cor <- sapply(train_data[, -which(names(train_data) == ClassFeature_)], function(col) jaccard(col, train_data[ClassFeature_]==PositiveClass_))
      }
      # else if (Sorted == "LASSO") { 
      #   model_lasso <- glmnet::cv.glmnet(x=data.matrix(train_data[, -which(names(train_data) == ClassFeature_)]), y = train_data[ClassFeature_]==PositiveClass_, alpha = 1, lambda = 10^seq(3, -2, by = -.1), maxit=10000000, standardize = TRUE, nfolds = 5, family = "binomial")
      #   coef <- as.matrix(coef(model_lasso, s = "lambda.min")) # get importance
      #   coef <- rownames(coef)[order(-abs(coef))] # order from high to low
      #   coef <- coef[-which(coef == "(Intercept)")] # remove intercept
      # }
      
      coef <- names(cor)[order(-abs(cor))] 
      train_data <- train_data[,c(coef,ClassFeature_)] # sort data features by LASSO importance
    }
    
    saveData(output_path, train_data, file_name)
  }
  
  # Create selected settings 
  settings_path <- settingsExplore(settings, 
                                   output_path = output_path, # C++ cannot handle spaces in file path well, avoid those
                                   file_name = file_name,
                                   train_data = train_data,
                                   OutputFile = OutputFile, 
                                   StartRulelength = StartRulelength,
                                   EndRulelength = EndRulelength,
                                   OperatorMethod = OperatorMethod,
                                   CutoffMethod = CutoffMethod,
                                   ClassFeature = ClassFeature,
                                   PositiveClass = PositiveClass,
                                   FeatureInclude = FeatureInclude,
                                   Maximize = Maximize,
                                   Accuracy = Accuracy,
                                   BalancedAccuracy = BalancedAccuracy,
                                   Specificity = Specificity,
                                   PrintSettings = PrintSettings,
                                   PrintPerformance = PrintPerformance,
                                   PrintCutoffSets = PrintCutoffSets,
                                   Subsumption = Subsumption,
                                   BranchBound = BranchBound,
                                   Parallel = Parallel,
                                   OutputMethod = OutputMethod,
                                   BinaryReduction = BinaryReduction)
  
  # Train EXPLORE model
  # TODO: allow to enter settings file instead of path?
  runExplore(settings_path)
  
  # Load results file
  settings <- paste(readLines(settings_path), collapse="\n")
  results <- paste(readLines(getSetting(settings, "OutputFile", type = "value")), collapse="\n")
  
  cand_models_lines <- strsplit(results, "\n")
  candidate_models <- grep("Candidate model:", unlist(cand_models_lines), value = TRUE)
  length(candidate_models)
  cutoff_sets<- grep("Total Count Cutoff Sets:", unlist(cand_models_lines), value = TRUE)
  result <- list("candidate_models" = candidate_models,
                 "cutoff_sets" = cutoff_sets)
  
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