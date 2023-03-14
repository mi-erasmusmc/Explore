# TODO: add descriptions for package manual

#' trainExplore This function finds the best decision rule for the given dataset based using EXPLORE. 
#'
#' @param output_path 
#' @param train_data 
#' @param settings_path 
#' @param file_name 
#' @param ... 
#'
#' @return
#' @export
#' @import Rcpp RcppParallel
#'
#' @examples
trainExplore <- function(output_path, train_data = NULL, settings_path = NULL, file_name = "train_data", ...) {
  
  # Create output folder
  if(!endsWith(output_path, "/")) {
    warning("Output path should end with /, add this")
    output_path <- paste0(output_path, "/")
  }
  
  if (!file.exists(output_path)) {dir.create(output_path, recursive = TRUE)}
  
  # Create project setting
  if (is.null(settings_path)) {
    if (is.null(train_data)) {
      stop("Data is NULL, but is required when settings_path is not entered.")
    } else {
      # Load template
      settings <- paste0(paste(readLines(paste0(system.file(package="Explore"),"/settings/template.project")), collapse="\n"),"\n")
    }
  }
  else {
    # Load settings
    settings <- paste0(paste(readLines(settings_path), collapse="\n"),"\n")
  }
  
  # TODO: check train_data and correct if necessary?
  # drop nearly constant or extremely sparse values
  
  # Save train_data if entered
  if (!is.null(train_data)) {
    saveData(output_path, train_data, file_name)
  }
  
  # Create selected settings 
  settings_path <- settingsExplore(settings, output_path, file_name, train_data, ...)
  
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


#' settingsExplore
#'
#' @param settings 
#' @param output_path 
#' @param file_name 
#' @param train_data 
#' @param maxRuleLength 
#' @param mandatoryFeatures 
#'
#' @return
settingsExplore <- function(settings,
                            output_path, # C++ cannot handle spaces in file path well, avoid those
                            file_name,
                            train_data = NULL,
                            OutputFile = NULL, 
                            StartRulelength = NULL,
                            EndRulelength = NULL,
                            OperatorMethod = NULL,
                            CutoffMethod = NULL,
                            ClassFeature,
                            PositiveClass,
                            FeatureInclude = NULL,
                            Maximize = NULL,
                            Accuracy = NULL,
                            Specificity = NULL,
                            PrintSettings = NULL,
                            PrintPerformance = NULL,
                            Subsumption = NULL,
                            BranchBound = NULL,
                            Parallel = NULL) {
  
  # Insert location training data and cutoff file if train_data is entered
  if (!is.null(train_data)) {
    settings <- changeSetting(settings, parameter = "DataFile", input = paste0(output_path, file_name, ".arff"), default_setting = NA)
    settings <- changeSetting(settings, parameter = "CutoffFile", input = paste0(output_path, file_name, ".cutoff"), default_setting = NA)
    
    settings <- changeSetting(settings, parameter = "ClassFeature", input = ClassFeature, default_setting = NA)
    settings <- changeSetting(settings, parameter = "PositiveClass", input = PositiveClass, default_setting = NA)
  }
  
  # Insert other settings if given and default if @ in template
  settings <- changeSetting(settings, parameter = "OutputFile", input = OutputFile, default_setting = paste0(output_path, file_name, ".result"))
  settings <- changeSetting(settings, parameter = "StartRulelength", input = StartRulelength, default_setting = 1)
  settings <- changeSetting(settings, parameter = "EndRulelength", input = EndRulelength, default_setting = 3)
  settings <- changeSetting(settings, parameter = "OperatorMethod", input = OperatorMethod, default_setting = "EXHAUSTIVE")
  settings <- changeSetting(settings, parameter = "CutoffMethod", input = CutoffMethod, default_setting = "RVAC")
  settings <- changeSetting(settings, parameter = "FeatureInclude", input = FeatureInclude, default_setting = "")
  settings <- changeSetting(settings, parameter = "Maximize", input = Maximize, default_setting = "ACCURACY")
  settings <- changeSetting(settings, parameter = "Accuracy", input = Accuracy, default_setting = "custom")
  settings <- changeSetting(settings, parameter = "Specificity", input = Specificity, default_setting = "")
  settings <- changeSetting(settings, parameter = "PrintSettings", input = PrintSettings, default_setting = "yes")
  settings <- changeSetting(settings, parameter = "PrintPerformance", input = PrintPerformance, default_setting = "yes")
  settings <- changeSetting(settings, parameter = "Subsumption", input = Subsumption, default_setting = "yes")
  settings <- changeSetting(settings, parameter = "BranchBound", input = BranchBound, default_setting = "yes")
  settings <- changeSetting(settings, parameter = "Parallel", input = Parallel, default_setting = "no")
  
  # Save settings file
  settings_path <- paste0(output_path, file_name,".project")
  write.table(settings, 
              file=settings_path, 
              quote = FALSE,
              col.names = FALSE,
              row.names = FALSE)
  
  return(settings_path)
}

#' predictExplore
#'
#' @param model 
#' @param test_data 
#'
#' @return
#' @export
#'
#' @examples
predictExplore <- function(model, test_data) {
  
  # Return NULL if cannot make predictions
  if (is.null(model) || model == "") {
    return(NULL)
  }
  
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
  
  colnames(data_model) <- all_terms
  predictions <- as.integer(rowSums(data_model)>0)
  
  return(predictions)
}


#' aucrocExplore
#'
#' @param output_path 
#' @param train_data 
#' @param settings_path 
#' @param file_name 
#' @param ... 
#'
#' @return
#' @export
#'
#' @examples
aurocEXPLORE <- function(output_path, train_data, settings_path, file_name, ...) {
  # TODO: check with latest implementation in PLP
  
  # Range of specificities to check
  specificities <- seq(from = 0.01, to = 0.99, by = 0.02)
  
  # Set specificity constraint and maximize sensitivity
  sensitivities <- rep(NA, length(specificities))
  for (s in 1:length(specificities)) { # s <- 0.1
    
    model <- Explore::trainExplore(output_path = output_path, train_data = train_data, settings_path = settings_path, Maximize = "SENSITIVITY", Specificity = specificities[s], ...)
    
    # Extract sensitivity from results file
    results <- paste(readLines(paste0(output_path, "train_data.result")), collapse="\n")
    
    sensitivity <- stringr::str_extract_all(results, "Train-set: .*?\u000A")[[1]]
    sensitivity <- stringr::str_extract(results, "SE:.*? ")[[1]]
    sensitivity <- stringr::str_remove_all(sensitivity, "SE:")
    sensitivity <- stringr::str_replace_all(sensitivity, " ", "")
    
    sensitivities[s] <- as.numeric(sensitivity)
  }
  
  auroc <- simple_auc(TPR = rev(sensitivities), FPR = rev(1 - specificities))
  # plot(1-specificities, sensitivities)
  
  return(auroc)
}
