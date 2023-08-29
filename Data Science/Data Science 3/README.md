# Exercise 3: Customer Segmentation Analysis

This README provides an overview of the customer segmentation analysis code, which aims to segment customers based on their attributes using K-means clustering.

## Overview

The provided Python script performs the following tasks:

1. Data Preprocessing: Loads the dataset, explores its structure, checks data types, handles missing values, and encodes categorical variables.
2. Outlier Handling (Optional): Detects and removes outliers from numeric columns using the z-score method.
3. Data Visualization: Creates visualizations such as box plots, histograms, scatter plots, and bar plots to understand the data distribution and relationships.
4. Feature Scaling: Standardizes numeric features using StandardScaler and encodes categorical features using OneHotEncoder.
5. K-means Clustering: Performs K-means clustering with a range of clusters to determine the optimal number of clusters (Elbow Curve). Chooses the optimal number of clusters and assigns cluster labels to the original dataset.
6. Cluster Analysis: Calculates the mean values of numeric features for each cluster and visualizes the results using a bar plot.

## Usage

1. Make sure you have the required libraries installed. You can use the following command to install them:

```shell
pip install pandas numpy matplotlib seaborn scikit-learn
```


2. Place the dataset file "JustBuy_data.csv" in the same directory as the script.

3. Run the script using a Python interpreter. It will execute the data preprocessing, visualization, clustering, and cluster analysis steps.

## Code Structure

The code is organized into several sections:

- **Data Preprocessing**: Loads the dataset, explores its structure, checks data types, handles missing values, and encodes categorical variables.

- **Outlier Handling** (Optional): Detects and removes outliers from numeric columns using the z-score method.

- **Data Visualization**: Creates various visualizations such as box plots, histograms, scatter plots, and bar plots to understand the data distribution and relationships.

- **Feature Scaling**: Standardizes numeric features using StandardScaler and encodes categorical features using OneHotEncoder.

- **K-means Clustering**: Performs K-means clustering with a range of clusters to determine the optimal number of clusters (Elbow Curve). Chooses the optimal number of clusters and assigns cluster labels to the original dataset.

- **Cluster Analysis**: Calculates the mean values of numeric features for each cluster and visualizes the results using a bar plot.

## Output

The script generates console outputs for data exploration, visualization, clustering analysis, and cluster analysis. Additionally, it outputs visualizations of data distribution and relationships, as well as the elbow curve for determining the optimal number of clusters.

## Note

This script provides a basic example of customer segmentation analysis using K-means clustering. Depending on your specific use case, you may need to adapt the code to your data and analytical goals.

Feel free to modify and extend the code to fit your project requirements.

For any questions or clarifications, please refer to the code comments or reach out for assistance.
