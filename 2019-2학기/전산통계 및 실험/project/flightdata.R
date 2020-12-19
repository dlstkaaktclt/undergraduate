library(readr)
library(dplyr)
library(RSQLite)
library(tidyverse)
con <- dbConnect(SQLite(), "project.sqlite")

# part 1-2


# read data from csv files.


airlines <- read_csv("data/airlines.csv", col_types = cols(
    Code = col_character(),
    Description = col_character()))


airports <- read_csv("data/airports.csv", col_names = c(
    "Airport_ID", "Name", "City", "Country", "IATA", "ICAO", "Latitude", "Longitude", "Altitude_ft","Timeoffset_UTC","DST","Timezone","Type", "Source"), col_types = cols(
        Airport_ID = col_integer(),
        Name = col_character(),
        City = col_character(),
        Country = col_character(),
        IATA = col_character(),
        ICAO = col_character(),
        Latitude = col_double(),
        Longitude = col_double(),
        Altitude_ft = col_double(),
        Timeoffset_UTC = col_double(),
        DST = col_factor(),
        Timezone = col_character(),
        Type = col_factor(),
        Source = col_factor()),
    na = c("", "\\N")
    )

airplanes <- read_csv("data/airplanes.csv", col_types = cols(
    TailNum = col_character(),
    Type = col_factor(),
    Manufacturer = col_character(),
    IssueDate = col_character(), # col_date(format = "%m/%d/%y") (date type) is not correctly applied by dbWriteTable. saved as double value.. so use col_character().
    Model = col_character(),
    Status = col_factor(),
    AircraftType = col_factor(),
    EngineType = col_factor(),
    Year = col_integer()),
    na = c("None", ""))


# make table at sqlite

dbWriteTable(con, "airlines", airlines, overwrite = TRUE)
dbWriteTable(con, "airports", airports, overwrite = TRUE)
dbWriteTable(con, "airplanes", airplanes, overwrite = TRUE)


# remove table from environment to save memory

rm(airlines)
rm(airports)
rm(airplanes)


# part 1-3

for (i in 2001:2018) {
    
    for (j in 1:12) {
        filepath = str_c("data/", toString(i), ifelse(j < 10, str_c("0",toString(j)),toString(j)), ".zip")
        temp_table <- read_csv(filepath, 
            col_names = c(
                "Year",
                "Month",
                "Day_of_Month",
                "Day_Of_Week",
                "Dep_Time",
                "CRS_Dep_Time",
                "Arr_Time",
                "CRS_Arr_Time",
                "Op_Unique_Carrier",
                "OP_Carrier_FL_Num",
                "Tail_Num",
                "Actual_Elapsed_Time",
                "CRS_Elapsed_Time",
                "Air_Time",
                "Arr_Delay",
                "Dep_Delay",
                "Origin",
                "Dest",
                "Distance",
                "Taxi_In",
                "Taxi_Out",
                "Cancelled",
                "Cancellation_Code",
                "Diverted",
                "Carrier_Delay",
                "Weather_Delay",
                "NAS_Delay",
                "Security_Delay",
                "Late_Aircraft_Delay",
                "dummy"
            ), skip = 1,
            col_types = cols(
            Year = col_integer(),
            Month = col_integer(),
            Day_of_Month = col_integer(),
            Day_Of_Week = col_integer(),
            Dep_Time = col_integer(),
            CRS_Dep_Time = col_integer(),
            Arr_Time = col_integer(),
            CRS_Arr_Time = col_integer(),
            Op_Unique_Carrier = col_character(),
            OP_Carrier_FL_Num = col_character(),
            Tail_Num = col_character(),
            Actual_Elapsed_Time = col_double(),
            CRS_Elapsed_Time = col_double(),
            Air_Time = col_double(),
            Arr_Delay = col_double(),
            Dep_Delay = col_double(),
            Origin = col_character(),
            Dest = col_character(),
            Distance = col_double(),
            Taxi_In = col_double(),
            Taxi_Out = col_double(),
            Cancelled = col_double(),
            Cancellation_Code = col_factor(),
            Diverted = col_double(),
            Carrier_Delay = col_double(),
            Weather_Delay = col_double(),
            NAS_Delay = col_double(),
            Security_Delay = col_double(),
            Late_Aircraft_Delay = col_double(),
            dummy = col_character()), progress = FALSE)
        
        # because of extra comma at csv file, we need to truncate last column
        
        temp_table <- temp_table %>% select(-ncol(temp_table))
        
        if(i==2001 & j == 1)
        {
            dbWriteTable(con, "flights", temp_table, overwrite = TRUE)
        }
        else
        {
            dbWriteTable(con, "flights", temp_table, append = TRUE)
        }
        
        
        # check progressing
        print(str_c(filepath, " import success"))
        
    }
    
}

# part 1-4


# lot of index make database slower at insert, delete, update.
# so, add indices really needed.


res <- dbSendQuery(con, "CREATE INDEX origin ON flights(Origin)")
dbClearResult(res)
res <- dbSendQuery(con, "CREATE INDEX dest ON flights(Dest)")
dbClearResult(res)
res <- dbSendQuery(con, "CREATE INDEX date ON flights(Year, Month, Day_of_Month)")
dbClearResult(res)
res <- dbSendQuery(con, "CREATE INDEX tail_num ON flights(Tail_Num)")
dbClearResult(res)
res <- dbSendQuery(con, "CREATE INDEX tailnum ON airplanes(TailNum)")
dbClearResult(res)
res <- dbSendQuery(con, "CREATE INDEX code ON airlines(Code)")
dbClearResult(res)


dbDisconnect(con)




