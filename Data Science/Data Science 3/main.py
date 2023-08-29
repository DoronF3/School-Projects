import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.preprocessing import StandardScaler, OneHotEncoder, OrdinalEncoder
from sklearn.compose import ColumnTransformer
from sklearn.cluster import KMeans

# Load the dataset
df = pd.read_csv('JustBuy_data.csv')

# Check the structure of the dataset
print(df.head())

# Check data types
print(df.dtypes)

# Summary statistics
print(df.describe())

# Check for missing values
print(df.isnull().sum())

# Drop the customer id column
df = df.drop("Customer_Id", axis=1)

# loop through all columns in the dataframe
for col in df.columns:
    # check if the column type is numeric
    if df[col].dtype == 'int64' or df[col].dtype == 'float64':
        # fill the missing values with the mean of the column
        df[col].fillna(df[col].mean(), inplace=True)
    else:
        # fill the missing values with 'Unknown'
        df[col].fillna('Unknown', inplace=True)

# Select numerical and categorical variables
numeric_cols = ['Age', 'Annual_Income_(k$)', 'Distance_to_Store_(km)', 'Online_Shopping_Score',
                'Purchase_Diversity', 'Online_Engagement_Score', 'Social_Media_Influence',
                'Customer_Lifetime_Value', 'Discount_Sensitivity']

categorical_cols = ['Gender', 'Education_Level', 'Marital_Status', 'Number_of_Children', 'Shopping_Persona']

# Box plots to identify outliers
fig, axes = plt.subplots(nrows=3, ncols=3, figsize=(15, 10))
axes = axes.flatten()

for i, var in enumerate(numeric_cols):
    ax = axes[i]
    sns.boxplot(data=df, x=var, ax=ax)
    ax.set_title(var)

plt.tight_layout()
plt.show()

# Handling outliers (optional)
# For example, removing outliers using z-score method
from scipy.stats import zscore

z_scores = zscore(df[numeric_cols])
outliers = (z_scores > 3) | (z_scores < -3)
df = df[~outliers.any(axis=1)]

# Confirm the removal of outliers
print("Number of rows after removing outliers:", len(df))

# Statistical summaries
print(df.describe())  # Descriptive statistics for numerical variables
# loop through all columns in the dataframe
for col in df.columns:
    # check if the column type is numeric
    if df[col].dtype == 'object':
        value_counts = df[col].value_counts()
        print(value_counts)  # Frequency distribution for categorical variable
        # Create a bar plot for the frequency distribution
        plt.figure(figsize=(10, 6))
        plt.bar(value_counts.index, value_counts.values)
        plt.xlabel(col)
        plt.ylabel('Frequency')
        plt.title(f'Frequency Distribution of {col}')
        plt.xticks(rotation=90)
        plt.show()

# Visualization
sns.histplot(df['Age'], kde=True)  # Histogram with KDE for Age
plt.xlabel('Age')
plt.ylabel('Frequency')
plt.title('Distribution of Age')
plt.show()

sns.boxplot(x='Gender', y='Annual_Income_(k$)', data=df)  # Box plot for Annual Income by Gender
plt.xlabel('Gender')
plt.ylabel('Annual Income (k$)')
plt.title('Annual Income Distribution by Gender')
plt.show()

# Box Plot: Relationship between 'Customer_Lifetime_Value' and 'Gender'
plt.figure(figsize=(8, 6))
sns.boxplot(data=df, x='Gender', y='Customer_Lifetime_Value')
plt.xlabel('Gender')
plt.ylabel('Customer Lifetime Value')
plt.title('Relationship between Gender and Customer Lifetime Value')
plt.show()

# Box Plot: Relationship between 'Customer_Lifetime_Value' and 'Age'
plt.figure(figsize=(8, 6))
sns.boxplot(data=df, x='Age', y='Customer_Lifetime_Value')
plt.xlabel('Age')
plt.ylabel('Customer Lifetime Value')
plt.title('Relationship between Age and Customer Lifetime Value')
plt.show()

sns.scatterplot(x='Online_Shopping_Score', y='Purchase_Diversity', hue='Shopping_Persona', data=df)  # Scatter plot with hue for Shopping Persona
plt.xlabel('Online Shopping Score')
plt.ylabel('Purchase Diversity')
plt.title('Relationship between Online Shopping Score and Purchase Diversity')
plt.legend(loc='best')
plt.yticks(range(int(df['Purchase_Diversity'].min()), int(df['Purchase_Diversity'].max())+1))
plt.show()

# Create preprocessor for numerical and categorical variables
numeric_transformer = StandardScaler()
categorical_transformer = OneHotEncoder(drop='first')

# Apply transformations
preprocessor = ColumnTransformer(
    transformers=[
        ('num', numeric_transformer, numeric_cols),
        ('cat', categorical_transformer, categorical_cols)
    ])

# Preprocess the data
X = preprocessor.fit_transform(df)

# Perform K-means clustering with a range of clusters
inertia = []
k_values = range(2, 20)
for k in k_values:
    kmeans = KMeans(n_clusters=k, random_state=42)
    kmeans.fit(X)
    inertia.append(kmeans.inertia_)

# Plot the elbow curve
plt.plot(inertia, k_values, 'bo-')
plt.ylabel('Number of Clusters (k)')
plt.xlabel('Within-Cluster Sum of Squares (Inertia)')
plt.title('Elbow Curve')
plt.yticks(range(min(k_values), max(k_values)+1, 1))
plt.show()

# Choose the optimal number of clusters
k_optimal = 14

# Perform K-means clustering
kmeans = KMeans(n_clusters=k_optimal, random_state=42)
kmeans.fit(X)

# Assign cluster labels to the original dataset
df['Cluster_Labels'] = kmeans.labels_

# Analyze the clusters
cluster_analysis = df.groupby('Cluster_Labels')[numeric_cols].mean()

# Plot the cluster analysis using a bar plot
cluster_analysis.plot(kind='bar', stacked=True, figsize=(10, 6))
plt.xlabel('Cluster Labels')
plt.ylabel('Mean Value')
plt.title('Cluster Analysis')
plt.legend(loc='best')
plt.show()

# Set the display options to show all columns and rows
pd.set_option('display.max_columns', None)
pd.set_option('display.max_rows', None)

print(cluster_analysis.info)