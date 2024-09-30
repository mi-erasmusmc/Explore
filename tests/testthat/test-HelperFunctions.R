test_that("Convert logical to 0/1", {
  train_data <- data.frame(check.names = FALSE,
                           outcomeCount = c(FALSE,FALSE,FALSE,
                                            FALSE,FALSE,TRUE),
                           `198124209` = c(FALSE,FALSE,FALSE,
                                           FALSE,FALSE,TRUE),
                           `316139209` = c(FALSE,FALSE,FALSE,
                                           FALSE,FALSE,FALSE),
                           `316139210` = c(FALSE,FALSE,FALSE,
                                           FALSE,FALSE,FALSE)
  )
  
  train_data <- convert_logical(train_data)
  
  expect_true(all(sapply(train_data, function(col) all(col %in% c(0, 1)))))
  
})
