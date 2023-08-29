# Customer Retention Prediction README

This README provides an overview of the customer retention prediction code, which aims to predict customer retention using machine learning models.

## Overview

The provided Python script performs the following tasks:

1. Data Preprocessing: Loads the dataset, performs data exploration, handles missing values, and encodes categorical variables.
2. Model Training: Trains three different classification models (Logistic Regression, Decision Tree, Random Forest) on the preprocessed data.
3. Model Evaluation: Evaluates the models' performance using various metrics such as accuracy, precision, recall, F1-score, and ROC-AUC.
4. Hyperparameter Tuning: Uses grid search to find the optimal hyperparameters for the Random Forest model.
5. Feature Importance: Calculates and visualizes feature importances for the optimized Random Forest model.

## Usage

1. Make sure you have the required libraries installed. You can use the following command to install them:

```shell
pip install numpy pandas matplotlib scikit-learn tabulate
```

2. Place the dataset file "RetentionBusinessCustomers.csv" in the same directory as the script.

3. Run the script using a Python interpreter. It will execute the data preprocessing, model training, evaluation, hyperparameter tuning, and feature importance steps.

## Code Structure

The code is organized into several sections:

- **Data Preprocessing**: Loads the dataset, explores its structure, checks data types, handles missing values, and encodes categorical columns.

- **Model Training and Evaluation**: Trains Logistic Regression, Decision Tree, and Random Forest models on the preprocessed data. Evaluates the models' performance using metrics such as accuracy, precision, recall, F1-score, and ROC-AUC.

- **Hyperparameter Tuning**: Performs grid search to find the best hyperparameters for the Random Forest model.

- **Feature Importance**: Calculates feature importances for the optimized Random Forest model and visualizes them using a bar plot.

## Output

The script generates console outputs for data exploration, model evaluation, hyperparameter tuning results, and feature importance analysis. Additionally, it outputs visualizations of feature importances.

## Note

This script is a basic example of customer retention prediction using machine learning techniques. Depending on your specific use case, you may need to adapt the code to your data and analytical goals.

Feel free to modify and extend the code to fit your project requirements.

For any questions or clarifications, please refer to the code comments or reach out for assistance.
