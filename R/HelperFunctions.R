#' @export
getSetting <- function(settings, parameter, type = "value") {
  extraction <- stringr::str_extract(settings, paste0(parameter , "=.*?\u000A"))[[1]] # check if it works with  multiple best candidates
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

#' @export
changeSetting <- function(settings, parameter, input, default_setting) {
  
  current_setting <- getSetting(settings, parameter, type = "complete")
  
  if (!is.null(input)) { # If input has a value -> update parameter in settings
    settings <- sub(current_setting, paste0(parameter, "=", input, ""), settings)
  
  } else if  (grepl(paste0("@",parameter), settings, fixed=TRUE)) { # If value is required in settings, use default value if possible
    # TODO: extend list of patterns that are recognized as required settings
    
    if(!is.na(default_setting)) {
      settings <- sub(current_setting, paste0(parameter, "=", default_setting, ""), settings)
    } else{
      warning(paste0('Default setting for ', parameter, ' is NA, but setting is required to replace @', parameter, "."))
    }
  }
  
  return(settings)
}

#' saveData
#'
#' @param output_path 
#' @param train_data 
#' @param file_name 
#'
#' @return
#' @export
saveData <- function(output_path, train_data, file_name) {
  
  # Save data as arff file
  if (file.exists(paste0(output_path, file_name, ".arff"))) {file.remove(paste0(output_path, file_name, ".arff"))}
  farff::writeARFF(train_data, paste0(output_path, file_name, ".arff"))
  
  # Create cutoff file based on column names
  cutoff <- paste(colnames(train_data)[1:(ncol(train_data)-1)], collapse = ",")
  write.table(cutoff, 
              file=paste0(output_path, file_name,".cutoff"), 
              quote = FALSE,
              col.names = FALSE,
              row.names = FALSE)
  
  # TODO: Support other file formats?
}

simple_auc <- function(TPR, FPR){
  # inputs already sorted, best scores first 
  # TODO: different computation? is it same as standard packages (how LASSO computed)?
  dFPR <- c(diff(FPR), 0)
  dTPR <- c(diff(TPR), 0)
  sum(TPR * dFPR) + sum(dTPR * dFPR)/2
}
