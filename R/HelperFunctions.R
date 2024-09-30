#' Get the setting parameter value
#'
#' @param settings Settings parameter, string
#' @param parameter Parameter value, string
#' @param type Type value, string
#'
#' @return A parameter value, character.
#' @importFrom stringr str_extract str_replace_all
getSetting <- function(settings, parameter, type = "value") {
  extraction <- stringr::str_extract(settings, paste0(parameter , "=.*?\u000A"))[[1]] 
  extraction <- stringr::str_replace_all(extraction, "\\n", "")
  
  parameter_value <- stringr::str_replace(extraction, paste0(parameter, "="), "")
  parameter_value <- stringr::str_replace_all(parameter_value, " ", "")
  
  if (type == "value") {
    return(parameter_value)
  } else if (type == "complete") {
    return(extraction)
  } else {
    warning("Type is undefined.")
  }
}


#' Change setting parameter value
#'
#' @param settings Settings parameter, string
#' @param parameter Parameter value, string
#' @param input Input value
#' @param default_setting Default setting
#'
#' @return A setting parameter value
#' @importFrom utils write.table
changeSetting <- function(settings, parameter, input, default_setting) {
  
  current_setting <- getSetting(settings, parameter, type = "complete")
  
  if (is.na(current_setting)) {
    warning(paste0("Current setting for ", parameter, " not correctly retrieved from file."))
  }
  
  if (!is.null(input)) { # If input has a value -> update parameter in settings
    
    # if (input=="custom") {
    #   if (parameter=="Accuracy") {input <- custom_Accuracy()}
    # }
    
    settings <- sub(current_setting, paste0(parameter, "=", input, ""), settings)
  
  } else if  (grepl(paste0("@",parameter), settings, fixed=TRUE)) { # If value is required in settings, use default value if possible
    # TODO: extend list of patterns that are recognized as required settings
    
    if(!is.na(default_setting)) {
      
      # if (default_setting=="custom") {
      #   if (parameter=="Accuracy") {default_setting <- custom_Accuracy()}
      # }
      
      settings <- sub(current_setting, paste0(parameter, "=", default_setting, ""), settings)
    } else{
      warning(paste0('Default setting for ', parameter, ' is NA, but setting is required to replace @', parameter, "."))
    }
  }
  
  return(settings)
}

#' saveData
#'
#' @param output_path A string declaring the path to the settings
#' @param train_data Train data
#' @param file_name A string declaring the the path to the file name
#' 
#' @importFrom farff writeARFF
#' @importFrom utils write.table
saveData <- function(output_path, train_data, file_name) {
  
  # Fix col type for binary data
  binary_cols <- sapply(1:ncol(train_data), function(c) all(train_data[[c]] %in% 0:1))
  
  # Convert TRUE/FALSE to 1/0
  train_data <- convert_logical(train_data)

  # Order data (first binary then continuous features)
  train_data <- cbind(train_data[binary_cols],train_data[!binary_cols]) # Order needed for correct functioning of main algorithm in C++ 
  
  # Save data as arff file
  if (file.exists(paste0(output_path, file_name, ".arff"))) {file.remove(paste0(output_path, file_name, ".arff"))}
  farff::writeARFF(train_data, paste0(output_path, file_name, ".arff"))
  
  # Create cutoff file based on column names
  # TODO: add input to exclude class?
  # FOR PLP: cutoff <- paste(colnames(train_data)[2:ncol(train_data)], collapse = ",")
  cutoff <- paste(colnames(train_data)[1:(ncol(train_data)-1)], collapse = ",")
  write.table(cutoff, 
              file=paste0(output_path, file_name,".cutoff"), 
              quote = FALSE,
              col.names = FALSE,
              row.names = FALSE)
  
  # TODO: Support other file formats?
}

convert_logical <- function(train_data) {
  
  binary_cols <- sapply(train_data, function(col) all(col %in% c(0, 1, TRUE, FALSE)))
  
  # Convert TRUE/FALSE to 1/0 and create factors
  train_data[binary_cols] <- lapply(train_data[binary_cols], function(col) {
    col <- as.numeric(as.logical(col))  # Convert TRUE/FALSE to 1/0
    factor(col, levels = c(0, 1), labels = c(0, 1))  # Convert to factors
  })
  
  return(train_data)
  
}

# Correlation metric for binary data.
jaccard <- function(a, b) {
  intersection = length(intersect(a, b))
  union = length(a) + length(b) - intersection
  return (intersection/union)
}

phi <- function(a, b) {
  contingency_tb <- table(a, b)
  
  r.sum <- rowSums(contingency_tb)
  c.sum <- colSums(contingency_tb)
  
  total <- sum(r.sum)
  r.sum <- r.sum/total
  c.sum <- c.sum/total
  
  v <- prod(r.sum, c.sum)
  phi <- (contingency_tb[1,1] / total - c.sum[1] * r.sum[1] / sqrt(v))
  names(phi) <- NULL
  
  return(phi)
}



