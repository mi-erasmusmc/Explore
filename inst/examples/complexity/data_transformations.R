library(dplyr)

# Adapt original iris dataset
# Bin variables to have equal number of thresholds
data_path <- "~/Documents/Git/_Projects/Explore/inst/examples/tests/iris.arff"
data <- farff::readARFF(data_path)

data <- data %>% mutate_at(c("sepallength","sepalwidth","petallength","petalwidth"), .funs = list(ntile = ~ntile(.,3)))

# data %>%
#   group_by(vars(ends_with("ntile"))) %>%
#   summarise_at(c("sepallength","sepalwidth","petallength","petalwidth"), list(name = mean))

data <- data %>% group_by(sepallength_ntile) %>% mutate(sepallength=mean(sepallength))
data <- data  %>% group_by(sepalwidth_ntile) %>% mutate(sepalwidth=mean(sepalwidth))
data <- data  %>% group_by(petallength_ntile) %>% mutate(petallength=mean(petallength))
data <- data  %>% group_by(petalwidth_ntile) %>% mutate(petalwidth=mean(petalwidth))

data <- data %>% ungroup() %>% select(c("sepallength","sepalwidth","petallength","petalwidth", "class"))

save_path <- "~/Documents/Git/_Projects/Explore/inst/examples/complexity/continuous_4_small.arff"
farff::writeARFF(as.data.frame(data), save_path)
