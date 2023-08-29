# Exercise 1: NBA Data Analysis

This README provides an overview of the NBA data analysis code, which retrieves and processes NBA game and player statistics.

## Overview

The provided Python script performs the following tasks:

1. Downloads NBA game data and player statistics from the RapidAPI platform.
2. Converts JSON data to CSV format for easier manipulation.
3. Merges game data and player statistics.
4. Cleans and preprocesses the data, handling missing values.
5. Creates new features based on the existing data.
6. Analyzes team wins and calculates win and lose points.

## Usage

1. Make sure you have the required libraries installed. You can use the following command to install them:
   
```shell
pip install numpy pandas requests
```


2. Replace the RapidAPI key with your actual key in the `headers` dictionary.
3. Run the script using a Python interpreter. It will execute the data retrieval, processing, and analysis steps.

## Code Structure

The code is structured into several sections:

- **Data Retrieval**: The script uses the `requests` library to fetch NBA game and player statistics data from RapidAPI endpoints.

- **Data Conversion**: The retrieved JSON data is converted into CSV format using the `pd.json_normalize` function and saved as CSV files.

- **Data Merging**: The script merges game data and player statistics using the `merge` function from the `pandas` library.

- **Data Preprocessing**: The script preprocesses the merged data by handling missing values. It imputes missing values with appropriate values, such as the mean for numerical columns and 'Unknown' for categorical columns.

- **Feature Creation**: New features are created based on existing data, such as calculating player efficiency, calculating player three-point attempt and made ratios, and more.

- **Data Analysis**: The script analyzes team wins and calculates win and lose points for specific teams.

## Output

The script generates several CSV files throughout the process, each containing different stages of processed data. The final output includes files with added features, such as `merged_games_data_features.csv`, as well as results from the team wins analysis.

## Note

This script serves as a basic example of data retrieval, processing, and analysis. Depending on your specific use case, you may need to adapt the code to your data sources and analytical goals.

Feel free to modify and extend the code as needed for your project.

For any questions or clarifications, please refer to the code comments or reach out for assistance.
