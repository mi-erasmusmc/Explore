#' Finds decision rule using Explore
#' 
#' `trainExplore()` finds the best decision rule for the given data set based using EXPLORE. 
#'
#' @param train_data Train data
#' @param output_path A string declaring the path to the settings
#' @param settings_path A string declaring the path to the settings
#' @param output_path A string declaring the path to the settings 
#' @param file_name A string declaring the the path to the file name
#' @param train_data Train data
#' @param OutputFile A string declaring the path to the output file
#' @param StartRulelength Positive integer
#' @param EndRulelength Positive integer
#' @param OperatorMethod One of list with strings, e.g. list = "EXHAUSTIVE", ...
#' @param CutoffMethod One of list with strings, list = "RVAC", ...
#' @param ClassFeature  String, should be name of one of columns in data train. Always provided by the user. The string should be enclused in single quotation marks, e.g. 'class'
#' @param PositiveClass 1 or string (?) (should be one of elements of column 'ClassFeature' in data train). Always provided by the user. The string should be enclused in single quotation marks, e.g. 'class'
#' @param FeatureInclude Empty or string (should be name of one or more columns in data train separated by ;)
#' @param Maximize One of list with strings, list = "ACCURACY", "SENSITIVITY", "SPECIFICITY", ...
#' @param Accuracy Float 0-0.999 -> default = 0 (if 0, make empty = computationally more beneficial)
#' @param BalancedAccuracy Float 0-0.999 -> default = 0 (if 0, make empty = computationally more beneficial)
#' @param Specificity float 0-0.999, default = 0
#' @param PrintSettings True or False
#' @param PrintPerformance True or False
#' @param Subsumption True or False
#' @param BranchBound True or False
#' @param Sorted One of list with strings, e.g. "none", "jaccard", ... Sort features based on correlation with outcome variable, NOTE: only when train_data is entered
#' @param Parallel True or False
#'
#' @return Model
#' @export
#' @import Rcpp checkmate
#' @importFrom RcppParallel RcppParallelLibs
trainExplore <- function(train_data = NULL,
                         settings_path = NULL,
                         output_path = tempdir(), 
                         file_name = "train_data",
                         OutputFile = NULL, 
                         StartRulelength = 1,
                         EndRulelength = 3,
                         OperatorMethod = "EXHAUSTIVE",
                         CutoffMethod = "ALL",
                         ClassFeature = "'class'",
                         PositiveClass = "'Iris-versicolor'",
                         FeatureInclude = "",
                         Maximize = "BALANCEDACCURACY",
                         Accuracy = 0,
                         BalancedAccuracy = 0,
                         Specificity = 0,
                         PrintSettings = TRUE,
                         PrintPerformance = TRUE,
                         Subsumption = FALSE,
                         BranchBound = TRUE,
                         Sorted = "none",
                         Parallel = TRUE,
                         ParallelMethod = "ONE",
                         BinaryReduction = FALSE) {
  
  if (!dir.exists(output_path)) {
    dir.create(output_path, recursive = TRUE)
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
                    checkDouble(Accuracy),
                    checkDouble(BalancedAccuracy),
                    checkDouble(Specificity),
                    checkLogical(PrintSettings),
                    checkLogical(PrintPerformance),
                    checkLogical(Subsumption),
                    checkLogical(BranchBound),
                    checkString(Sorted),
                    checkLogical(Parallel),
                    checkString(ParallelMethod),
                    checkLogical(BinaryReduction),
                    add = errorMessage,
                    combine = "and"
  )
  checkmate::reportAssertions(collection = errorMessage)
  
  PrintSettings <- ifelse(PrintSettings == TRUE, "yes", "no")
  PrintPerformance <- ifelse(PrintPerformance == TRUE, "yes", "no")
  Subsumption <- ifelse(Subsumption == TRUE, "yes", "no")
  BranchBound <- ifelse(BranchBound == TRUE, "yes", "no")
  Parallel <- ifelse(Parallel == TRUE, "yes", "no")
  BinaryReduction <- ifelse(BinaryReduction == TRUE, "yes", "no")
  Accuracy <- ifelse(Accuracy == 0, "", Accuracy)
  BalancedAccuracy <- ifelse(BalancedAccuracy == 0, "", BalancedAccuracy)
  Specificity <- ifelse(Specificity == 0, "", Specificity)
  
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
      } else if (Sorted == "phi") { 
        cor <- sapply(train_data[, -which(names(train_data) == ClassFeature_)], function(col) phi(col, train_data[ClassFeature_]==PositiveClass_))
      } 
      # else if (Sorted == "LASSO") { 
      #   model_lasso <- glmnet::cv.glmnet(x=data.matrix(train_data[, -which(names(train_data) == ClassFeature_)]), y = train_data[ClassFeature_]==PositiveClass_, alpha = 1, lambda = 10^seq(3, -2, by = -.1), maxit=10000000, standardize = TRUE, nfolds = 5, family = "binomial")
      #   coef <- as.matrix(coef(model_lasso, s = "lambda.min")) # get importance
      #   coef <- rownames(coef)[order(-abs(coef))] # order from high to low
      #   coef <- coef[-which(coef == "(Intercept)")] # remove intercept
      # }
        
      coef <- names(cor)[order(-abs(cor))] 
      train_data <- train_data[,c(coef,ClassFeature_)] # sort data features by importance
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
                                   Subsumption = Subsumption,
                                   BranchBound = BranchBound,
                                   Parallel = Parallel,
                                   ParallelMethod = ParallelMethod,
                                   BinaryReduction = BinaryReduction)
  
  # Train EXPLORE model
  # TODO: allow to enter settings file instead of path?
  runExplore(settings_path)
  
  # Load results file
  settings <- paste(readLines(settings_path), collapse="\n")
  results <- paste(readLines(getSetting(settings, "OutputFile", type = "value")), collapse="\n")
  
  # Load model
  rule_string <- stringr::str_extract(results, "Best candidate \\(overall\\):.*?\u000A")
  
  # Clean string
  rule_string <- stringr::str_replace(rule_string, "Best candidate \\(overall\\):", "")
  rule_string <- stringr::str_replace_all(rule_string, " ", "")
  rule_string <- stringr::str_replace_all(rule_string, "\\n", "")
  
  return(model = rule_string)
}


#' Writes settings in a table for Explore
#' 
#' `settingsExplore()` defines the parameters used by Explore
#'
#' @param settings Settings parameter
#' @param output_path A string declaring the path to the settings 
#' @param file_name A string declaring the the path to the file name
#' @param train_data Train data
#' @param OutputFile A string declaring the path to the output file
#' @param StartRulelength Positive integer
#' @param EndRulelength Positive integer
#' @param OperatorMethod One of list with strings, e.g. list = "EXHAUSTIVE", ...
#' @param CutoffMethod One of list with strings, list = "RVAC", ...
#' @param ClassFeature  String, should be name of one of columns in data train.
#' @param PositiveClass 1 or string (?) (should be one of elements of column 'ClassFeature' in data train)
#' @param FeatureInclude Empty or string (should be name of one of columns in data train)
#' @param Maximize One of list with strings, list = "ACCURACY", ...
#' @param Accuracy Float 0-1 -> default = 0 (if 0, make empty = computationally more beneficial)
#' @param Specificity  float 0-1, default = 0
#' @param PrintSettings True or False
#' @param PrintPerformance True or False
#' @param Subsumption True or False
#' @param BranchBound True or False
#' @param Parallel True or False
#'
#' @return Settings path
#' @import checkmate
settingsExplore <- function(settings,
                            output_path, # C++ cannot handle spaces in file path well, avoid those
                            file_name,
                            train_data = NULL,
                            OutputFile = NULL, 
                            StartRulelength = 1,
                            EndRulelength = 3,
                            OperatorMethod = "EXHAUSTIVE",
                            CutoffMethod = NULL,
                            ClassFeature,
                            PositiveClass,
                            FeatureInclude = "",
                            Maximize = "BALANCEDACCURACY",
                            Accuracy = 0,
                            BalancedAccuracy = 0,
                            Specificity = 0,
                            PrintSettings = "yes",
                            PrintPerformance = "yes",
                            Subsumption = "no",
                            BranchBound = "yes",
                            Parallel = "yes",
                            ParallelMethod = "ONE",
                            BinaryReduction = "no") {
  
  
  # Insert location training data and cutoff file if train_data is entered
  if (!is.null(train_data)) {
    settings <- changeSetting(settings, parameter = "DataFile", input = paste0(output_path, file_name, ".arff"))
    settings <- changeSetting(settings, parameter = "CutoffFile", input = paste0(output_path, file_name, ".cutoff"))
    settings <- changeSetting(settings, parameter = "ClassFeature", input = ClassFeature, default_setting = NA)
    settings <- changeSetting(settings, parameter = "PositiveClass", input = PositiveClass, default_setting = NA)
  }
  
  # Insert other settings if given and default if @ in template
  settings <- changeSetting(settings, parameter = "OutputFile", input = OutputFile)
  settings <- changeSetting(settings, parameter = "StartRulelength", input = StartRulelength)
  settings <- changeSetting(settings, parameter = "EndRulelength", input = EndRulelength)
  settings <- changeSetting(settings, parameter = "OperatorMethod", input = OperatorMethod)
  settings <- changeSetting(settings, parameter = "CutoffMethod", input = CutoffMethod)
  settings <- changeSetting(settings, parameter = "FeatureInclude", input = FeatureInclude)
  settings <- changeSetting(settings, parameter = "Maximize", input = Maximize)
  settings <- changeSetting(settings, parameter = "Accuracy", input = Accuracy)
  settings <- changeSetting(settings, parameter = "BalancedAccuracy", input = BalancedAccuracy)
  settings <- changeSetting(settings, parameter = "Specificity", input = Specificity)
  settings <- changeSetting(settings, parameter = "PrintSettings", input = PrintSettings)
  settings <- changeSetting(settings, parameter = "PrintPerformance", input = PrintPerformance)
  settings <- changeSetting(settings, parameter = "Subsumption", input = Subsumption)
  settings <- changeSetting(settings, parameter = "BranchBound", input = BranchBound)
  settings <- changeSetting(settings, parameter = "Parallel", input = Parallel)
  settings <- changeSetting(settings, parameter = "ParallelMethod", input = ParallelMethod)
  settings <- changeSetting(settings, parameter = "BinaryReduction", input = BinaryReduction)
  
  # Save settings file
  settings_path <- paste0(output_path, file_name,".project")
  write.table(settings, 
              file=settings_path, 
              quote = FALSE,
              col.names = FALSE,
              row.names = FALSE)
  
  return(settings_path)
}

#' Prediction with Explore
#' 
#' `predictExplore()` executes a prediction using Explore 
#'
#' @param model Model to run predictions
#' @param test_data Test data
#' 
#' @import checkmate
#' @importFrom stringr str_split_fixed
#' @export
predictExplore <- function(model, test_data) {
  
  # Return NULL if cannot make predictions
  if (is.null(model) || model == "") {
    return(NULL)
  }
  
  # Clean string
  model <- stringr::str_remove_all(model, '\"')
  model <- stringr::str_replace_all(model, "=", "==") 
  model <- stringr::str_replace_all(model, "<=", "<") # to correct initial case <= -> <== -> <= 
  
  # Split string 
  all_terms <- stringr::str_split_fixed(model, "OR", n=Inf)
  
  # Classify new predictions according to decision rules
  data_model <- data.frame(row.names = rownames(test_data))
  
  for (term in all_terms) { # term <- all_terms[1]
    
    # Split term in literals 
    all_literals <- stringr::str_split_fixed(term, "AND", n=Inf)
    
    col <- integer(nrow(test_data))
    for (literal in all_literals) { # literal <- all_literals[1]
      
      # Create a binary variable for this rule
      col <- col + as.integer(eval(parse(text=paste0("test_data$", literal))))
    }
    
    data_model <- cbind(data_model, as.integer(col==length(all_literals)))
  }
  
  colnames(data_model) <- all_terms # TODO: CHECK HERE WHY DATA_MODEL NO COLUMNS
  predictions <- as.integer(rowSums(data_model)>0)
  
  return(predictions)
}


#' modelsCurveExplore # TODO: update documentation?
#'
#' @param output_path A string declaring the path to the settings
#' @param train_data Train data
#' @param settings_path A string declaring the path to the settings
#' @param file_name A string declaring the the path to the file name
#' @param ... List of arguments
#'
#' @import checkmate
#' @return models for different sensitivities/specificities
#' @export
modelsCurveExplore <- function(train_data = NULL,
                               settings_path = NULL,
                               output_path, 
                               file_name = "train_data",
                               OutputFile = NULL, 
                               StartRulelength = 1,
                               EndRulelength = 3,
                               OperatorMethod = "EXHAUSTIVE",
                               CutoffMethod = "ALL",
                               ClassFeature = "'class'",
                               PositiveClass = "'Iris-versicolor'",
                               FeatureInclude = "",
                               Maximize = "BALANCEDACCURACY",
                               Accuracy = 0,
                               BalancedAccuracy = 0,
                               Specificity = 0,
                               PrintSettings = TRUE,
                               PrintPerformance = TRUE,
                               Subsumption = FALSE,
                               BranchBound = TRUE,
                               Sorted = "none",
                               Parallel = TRUE,
                               ParallelMethod = "ONE",
                               BinaryReduction = FALSE) {
  # TODO: only input required variables?
  
  # Range of specificities to check
  constraints <- c(seq(0.05,0.65,0.1), seq(0.75,0.97,0.02))
  
  modelsCurve <- tryCatch({
    models <- sapply(constraints, function(constraint) {
      print(paste0("Model for specificity: ", as.character(constraint)))
      
      # Fit EXPLORE
      model <- Explore::trainExplore(output_path = file.path(output_path, "modelsCurve"), train_data = train_data,
                                     settings_path = settings_path, 
                                     file_name = paste0("explore_specificity", as.character(constraint)),
                                     OutputFile = OutputFile,
                                     StartRulelength = StartRulelength, EndRulelength = EndRulelength,
                                     OperatorMethod = OperatorMethod, CutoffMethod = CutoffMethod,
                                     ClassFeature = ClassFeature, PositiveClass = PositiveClass,
                                     FeatureInclude = FeatureInclude, Maximize = "SENSITIVITY",
                                     Accuracy = Accuracy, BalancedAccuracy = BalancedAccuracy, Specificity = constraint,
                                     PrintSettings = PrintSettings, PrintPerformance = PrintPerformance,
                                     Subsumption = Subsumption, BranchBound = BranchBound,
                                     Parallel = Parallel)
      
      return(model)
    })
  },
  finally = warning("No model for specificity.")
  )
  
  return(modelsCurve)
}


#' rocCurveExplore
#'
#' @return auc value for EXPLORE
#' @export
#' @importFrom caret confusionMatrix
#' @importFrom pracma trapz
rocCurveExplore <- function(modelsCurve, data, labels) { # labels <- cohort$outcomeCount
  
  # TODO: input checks?
  
  # Combine all these results
  curve_TPR <- c(1,0)
  curve_FPR <- c(1,0)
  
  for (c in length(modelsCurve):1) {
    model <- modelsCurve[c]
    
    # Predict using train and test
    predict <- tryCatch(as.numeric(Explore::predictExplore(model = model, test_data = data)))
    
    # Compute metrics
    conf_matrix <- table(factor(predict, levels = c(0,1)), factor(labels, levels = c(0,1))) # binary prediction
    performance <- caret::confusionMatrix(conf_matrix, positive = '1')
    
    curve_TPR[c+2] <- performance$byClass['Sensitivity']
    curve_FPR[c+2] <- 1 - performance$byClass['Specificity']
  }
  
  roc <- pracma::trapz(curve_FPR[length(curve_FPR):1],curve_TPR[length(curve_TPR):1])
  
  return (roc)
}
