import json

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from tabulate import tabulate
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, roc_auc_score
from sklearn.model_selection import GridSearchCV

# Load the dataset
dataset = pd.read_csv("RetentionBusinessCustomers.csv")

# Check the structure of the dataset
print(dataset.head())

# Check data types
print(dataset.dtypes)

# Summary statistics
print(dataset.describe())

# Check for missing values
print(dataset.isnull().sum())

# loop through all columns in the dataframe
for col in dataset.columns:
    # check if the column type is numeric
    if dataset[col].dtype == 'int64' or dataset[col].dtype == 'float64':
        # fill the missing values with the mean of the column
        dataset[col].fillna(dataset[col].mean(), inplace=True)
    else:
        # fill the missing values with 'Unknown'
        dataset[col].fillna('Unknown', inplace=True)

# Drop the 'LastName' column
dataset = dataset.drop('LastName', axis=1)

# Split the dataset into training and testing sets
X = dataset.drop(['Retention'], axis=1)  # Features
y = dataset['Retention']  # Target variable

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Apply necessary preprocessing steps
# Define the columns to be scaled (numeric columns)
numeric_cols = ["RiskScore", "Age", "LoyaltyYears", "AccountBalance", "ProductCount ", "IncomeEstimate", "ComplaintSatisfaction", "CreditCardPoints"]

# Define the columns to be encoded (categorical columns)
categorical_cols = ["Location", "Gender", "HasCreditCard", "ActiveStatus", "CardType"]

# Feature scaling
scaler = StandardScaler()
X_train[numeric_cols] = scaler.fit_transform(X_train[numeric_cols])
X_test[numeric_cols] = scaler.transform(X_test[numeric_cols])

# Label encoding for categorical columns
X_train = pd.get_dummies(X_train, columns=categorical_cols)
X_test = pd.get_dummies(X_test, columns=categorical_cols)

# Initialize models
logreg = LogisticRegression()
dt = DecisionTreeClassifier()
rf = RandomForestClassifier()

# Train each model
logreg.fit(X_train, y_train)
dt.fit(X_train, y_train)
rf.fit(X_train, y_train)

# Make predictions
logreg_pred = logreg.predict(X_test)
dt_pred = dt.predict(X_test)
rf_pred = rf.predict(X_test)

# Evaluate performance
logreg_accuracy = accuracy_score(y_test, logreg_pred)
dt_accuracy = accuracy_score(y_test, dt_pred)
rf_accuracy = accuracy_score(y_test, rf_pred)

logreg_precision = precision_score(y_test, logreg_pred)
dt_precision = precision_score(y_test, dt_pred)
rf_precision = precision_score(y_test, rf_pred)

logreg_recall = recall_score(y_test, logreg_pred)
dt_recall = recall_score(y_test, dt_pred)
rf_recall = recall_score(y_test, rf_pred)

logreg_f1 = f1_score(y_test, logreg_pred)
dt_f1 = f1_score(y_test, dt_pred)
rf_f1 = f1_score(y_test, rf_pred)

logreg_roc_auc = roc_auc_score(y_test, logreg_pred)
dt_roc_auc = roc_auc_score(y_test, dt_pred)
rf_roc_auc = roc_auc_score(y_test, rf_pred)

# Define the data as a list of lists
data = [
    ["Logistic Regression", logreg_accuracy, logreg_precision, logreg_recall, logreg_f1, logreg_roc_auc],
    ["Decision Tree", dt_accuracy, dt_precision, dt_recall, dt_f1, dt_roc_auc],
    ["Random Forest", rf_accuracy, rf_precision, rf_recall, rf_f1, rf_roc_auc]
]

# Define the headers for the table
headers = ["Model", "Accuracy", "Precision", "Recall", "F1-score", "ROC-AUC"]

# Print the table
print(tabulate(data, headers=headers, tablefmt="grid"))


# Define parameter grid for the selected model (Random Forest)
param_grid = {
    'n_estimators': [100, 200, 300],
    'max_depth': [None, 5, 10],
    'min_samples_split': [2, 5, 10]
}

# Perform grid search for hyperparameter optimization
grid_search = GridSearchCV(estimator=rf, param_grid=param_grid, cv=5)
grid_search.fit(X_train, y_train)

# Get the best parameters and score
best_params = grid_search.best_params_
best_score = grid_search.best_score_

# Evaluate optimized model on the testing data
optimized_model = grid_search.best_estimator_
optimized_pred = optimized_model.predict(X_test)

optimized_accuracy = accuracy_score(y_test, optimized_pred)
optimized_precision = precision_score(y_test, optimized_pred)
optimized_recall = recall_score(y_test, optimized_pred)
optimized_f1 = f1_score(y_test, optimized_pred)
optimized_roc_auc = roc_auc_score(y_test, optimized_pred)

# Define the data as a list of lists
data = [
    ["Random Forest", rf_accuracy, rf_precision, rf_recall, rf_f1, rf_roc_auc],
    ["Optimized Random Forest:", optimized_accuracy, optimized_precision, optimized_recall, optimized_f1, optimized_roc_auc]
]

# Print the table
print(tabulate(data, headers=headers, tablefmt="grid"))
print()
print("Best Parameters:", best_params)
print("Best Score:", best_score)

# Calculate feature importances
feature_importances = optimized_model.feature_importances_

print(len(X_train.columns), len(feature_importances))

# Check lengths of arrays
if len(X_train.columns) == len(feature_importances):
    # Create a DataFrame with feature importances
    importance_df = pd.DataFrame({'Feature': X_train.columns, 'Importance': feature_importances})

    # Sort the DataFrame by importance values
    importance_df = importance_df.sort_values(by='Importance', ascending=False)

    # Print the feature importance table
    print(importance_df)

    # Visualize feature importances
    plt.figure(figsize=(10, 6))
    plt.barh(importance_df['Feature'], importance_df['Importance'])
    plt.xlabel('Importance')
    plt.ylabel('Feature')
    plt.title('Feature Importances')
    plt.show()
else:
    print("Error: The lengths of X.columns and feature_importances do not match.")