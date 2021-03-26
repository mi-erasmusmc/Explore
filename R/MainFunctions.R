
#' trainExplore
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
trainExplore <- function(output_path, train_data = NULL, settings_path = NULL, file_name = "train_data", ...) {
  
  # Create output folder
  if (!file.exists(output_path)) {dir.create(output_path, recursive = TRUE)}
  
  # Create project setting
  if (is.null(settings_path)) {
    if (is.null(train_data)) {
      stop("Data is NULL, but is required when settings_path is not entered.")
    } else {
      # Load template
      settings <- paste(readLines("inst/template.project"), collapse="\n")
    }
  }
  else {
    # Load settings
    settings <- paste(readLines(settings_path), collapse="\n")
  }
  
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
  
  all_rules <- stringr::str_extract_all(results, "Best candidate:.*?\u000A")[[1]] # check if it works with  multiple best candidates
  
  # TODO: find out which model is the one of interest
  rule_string <- all_rules[[length(all_rules)]]
  
  # Clean string
  rule_string <- stringr::str_replace(rule_string, "Best candidate:", "")
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
                            output_path,
                            file_name,
                            train_data = NULL,
                            EndRulelength = NULL,
                            ClassFeature,
                            PositiveClass,
                            FeatureInclude = NULL,
                            PrintSettings = NULL,
                            PrintPerformance = NULL) {
  
  # Insert location training data and cuytoff file if train_data is entered
  if (!is.null(train_data)) {
    settings <- changeSetting(settings, parameter = "DataFile", input = paste0(output_path, file_name, ".arff"), default_setting = NA)
    settings <- changeSetting(settings, parameter = "CutoffFile", input = paste0(output_path, file_name, ".cutoff"), default_setting = NA)
    
    settings <- changeSetting(settings, parameter = "ClassFeature", input = ClassFeature, default_setting = NA)
    settings <- changeSetting(settings, parameter = "PositiveClass", input = PositiveClass, default_setting = NA)
  }
  
  # Insert default setting if needed
  settings <- changeSetting(settings, parameter = "OutputFile", input = NULL, default_setting = paste0(output_path, file_name, ".result"))
  
  # Insert other settings if given
  settings <- changeSetting(settings, parameter = "EndRulelength", input = EndRulelength, default_setting = 3)
  settings <- changeSetting(settings, parameter = "FeatureInclude", input = FeatureInclude, default_setting = "")
  settings <- changeSetting(settings, parameter = "PrintSettings", input = PrintSettings, default_setting = "yes")
  settings <- changeSetting(settings, parameter = "PrintPerformance", input = PrintPerformance, default_setting = "yes")
  
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
#' @param output_path 
#' @param model 
#' @param test_data 
#'
#' @return
#' @export
#'
#' @examples
predictExplore <- function(output_path, model, test_data) {
  
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


