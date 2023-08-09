import re
import datetime
import numpy as np
import pandas as pd
from scipy import stats
from sklearn.compose import ColumnTransformer
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.preprocessing import StandardScaler, LabelEncoder, OneHotEncoder
from sklearn.ensemble import GradientBoostingClassifier, RandomForestRegressor
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, roc_auc_score
from sklearn.linear_model import LinearRegression, Lasso
import matplotlib.pyplot as plt
import seaborn as sns
from tabulate import tabulate

# Load the dataset
df = pd.read_csv('games.csv')

# Data exploration and preprocessing
# -----------------------------------------------------

# Check the structure of the dataset
print(df.head())

# Check data types
print(df.dtypes)

# Summary statistics
print(df.describe())

# Check for missing values
print(df.isnull().sum())

# Drop irrelevant columns
df.drop(['AppID', 'About the game', 'Header image', 'Website',
         'Support url', 'Support email', 'Metacritic url',
         'Screenshots', 'Movies', 'Reviews', 'Score rank'],
        axis=1, inplace=True)

# Check data types
print(df.dtypes)

# Check for missing values
print(df.isnull().sum())

# Feature engineering
# -----------------------------------------------------

# Feature: Price range category
df['Price range'] = pd.cut(df['Price'],
                           bins=[float('-inf'), 0, 10, 20, 30, 40, 60, float('inf')],
                           labels=['Free', '$0.1-$10', '$10-$20', '$20-$30', '$30-$40', '$40-$60', '$60+'])

# Convert 'Release date' column to datetime format
df['Release date'] = pd.to_datetime(df['Release date'], format='mixed')


# Create 'Release by Quarter' column
df['Release by Year and Quarter'] = df['Release date'].dt.to_period('Q')

# Create 'Release by Quarter' column in the format 'Q4'
df['Release by Quarter'] = 'Q' + df['Release date'].dt.quarter.astype(str)

# Create a new feature 'Has_English' with default value 0
df['Has_English'] = 0

# Iterate over the 'Supported languages' column and update 'Has_English' to 1 if English is present
for index, languages in enumerate(df['Supported languages']):
    if 'English' in languages:
        df.loc[index, 'Has_English'] = 1

# Alternatively, if your datetime column is in string format
# You can convert it to datetime object first and then apply the above code
df['UNIX_Release_date'] = df['Release date']
df['UNIX_Release_date'] = df['Release date'].apply(lambda x: int(datetime.datetime.timestamp(x)))

# Extract the year from the 'datetime' column
df['Year'] = df['Release date'].dt.year

# Group by year and calculate the maximum peak CCU
max_peak_ccu_by_year = df.groupby('Year')['Peak CCU'].max()

# Merge the max peak CCU back to the original DataFrame
df = df.merge(max_peak_ccu_by_year, left_on='Year', right_index=True, suffixes=('', '_Max_Peak_CCU'))

# Rename the merged column
df = df.rename(columns={'Peak CCU_Max_Peak_CCU': 'Max Peak CCU'})

# Drop the intermediate 'Year' column if needed
df.drop('Year', axis=1, inplace=True)

# Create a new column to count the number of publishers for each entry
df['num_Publishers'] = df['Publishers'].str.split(','). \
    apply(lambda x: len(x) if isinstance(x, list) else 0)

# Create a new column to count the number of categories for each entry
df['num_Categories'] = df['Categories'].str.split(','). \
    apply(lambda x: len(x) if isinstance(x, list) else 0)

# Create a new column to count the number of genres for each entry
df['num_Genres'] = df['Genres'].str.split(','). \
    apply(lambda x: len(x) if isinstance(x, list) else 0)

# Create a new column to count the number of tags for each entry
df['num_Tags'] = df['Tags'].str.split(','). \
    apply(lambda x: len(x) if isinstance(x, list) else 0)

# Remove symbols from 'Supported languages' column
df['num_Supported languages'] = df['Supported languages'].str. \
    replace(r'\(|\)|\[|\]', '', regex=True)
df['num_Supported languages'] = df['num_Supported languages'].str. \
    replace(r'\s+', ' ', regex=True)

# Remove symbols from 'Full audio languages' column
df['num_Full audio languages'] = df['Full audio languages'].str. \
    replace(r'\(|\)|\[|\]', '', regex=True)
df['num_Full audio languages'] = df['num_Full audio languages'].str. \
    replace(r'\s+', ' ', regex=True)

# Create a new column to count the number of supported languages for each entry
df['num_Supported languages'] = df['num_Supported languages'].str. \
    split(', ').apply(lambda x: len(x) if isinstance(x, list) else 0)

# Create a new column to count the number of full audio languages for each entry
df['num_Full audio languages'] = df['num_Full audio languages'].str. \
    split(', ').apply(lambda x: len(x) if isinstance(x, list) and x[0] != "" else 0)

# Convert boolean columns to integers
df['Windows'] = df['Windows'].astype(int)
df['Mac'] = df['Mac'].astype(int)
df['Linux'] = df['Linux'].astype(int)

# Calculate the number of platforms
df['num_Platforms'] = df['Windows'] + df['Linux'] + df['Mac']

# Calculate rating ratio
df['Review Ratio'] = \
    np.where((df['Positive'] == 0) & (df['Negative'] == 0),
             0, df['Positive'] / (df['Positive'] + df['Negative']))

# Calculate price per playtime
# -----------------------------------------------------

# Calculate price per Average playtime forever
df['Price per Avg Playtime Forever'] = \
    np.where(df['Average playtime forever'] ==
             0, 0, df['Price'] / df['Average playtime forever'])

# Calculate price per Average playtime two weeks
df['Price per Avg Playtime Two Weeks'] = \
    np.where(df['Average playtime two weeks'] ==
             0, 0, df['Price'] / df['Average playtime two weeks'])

# Calculate price per Median playtime forever
df['Price per Median Playtime Forever'] = \
    np.where(df['Median playtime forever'] ==
             0, 0, df['Price'] / df['Median playtime forever'])

# Calculate price per Median playtime two weeks
df['Price per Median Playtime Two Weeks'] = \
    np.where(df['Median playtime two weeks'] ==
             0, 0, df['Price'] / df['Median playtime two weeks'])

# Data cleaning and final steps
# -----------------------------------------------------

# Get today's date
today = pd.Timestamp(datetime.date.today())

# Remove all games who were not released yet
df = df[df['Release date'] <= today]

# Rename 'Price' column to 'price_usd'
df = df.rename(columns={'Price': 'Price_(usd)'})

# Add 'Content_Warning' column
df['Content_Warning_Indicator'] = df['Notes']. \
    apply(lambda x: 1 if pd.notna(x) else 0)

# Remove 'Notes' column
df = df.drop('Notes', axis=1)

# Frequency distribution for game names
value_counts = df['Name'].value_counts()
print(value_counts)

# Get the names that appear more than once
duplicate_names = value_counts[value_counts > 1].index
print(duplicate_names)
print(len(duplicate_names))

counter = 0
# Iterate over the duplicate names
for name in duplicate_names:
    # Get the rows with the current name
    rows = df[df['Name'] == name]

    # Check if all rows are the same
    if not rows.equals(rows.iloc[0]):
        # Rows are not the same, remove all the rows with the current name
        df = df[df['Name'] != name]
        counter += 1

    else:
        df = df.drop(rows.index[1:])

print(counter)

# Convert Developers column to string type
df['Developers'] = df['Developers'].astype(str)

# List of appearances of play tests, alphas, and betas
test_arr = [', Inc.', ', LLC', ', Inc', ', LTD.', ', LTD', ',LTD', ',Inc', ',Inc.', ',Ltd.']

# Create a single string pattern
pattern = '|'.join(re.escape(term) for term in test_arr)

# Remove play tests, alphas, and betas from the data
df['Developers'] = df['Developers'].str.replace(pattern, '', flags=re.IGNORECASE, regex=True)

# Convert Publishers column to string type
df['Publishers'] = df['Publishers'].astype(str)

# Remove play tests, alphas, and betas from the data
df['Publishers'] = df['Publishers'].str.replace(pattern, '', flags=re.IGNORECASE, regex=True)

# Convert Name column to string type
df['Name'] = df['Name'].astype(str)

# List of appearances of play tests, alphas, and betas
test_arr = ['Playtest', 'playtest', 'Play Test', 'Dedicated Server', 'Beta', 'BETA', 'beta', '(Alpha', 'Pre-Alpha',
            'pre-alpha', '(alpha)', 'Alpha)', '[Alpha', '[Play Test]', 'Test Version', 'Closed Alpha'
            'Pre-Season Preview', 'Multiplayer Test', 'DevTest', 'Technical Test', 'Free Multiplayer session']

# Create a single string pattern
pattern = '|'.join(re.escape(term) for term in test_arr)

# Remove play tests, alphas, and betas from the data
df = df[~df['Name'].str.contains(pattern, flags=re.IGNORECASE, regex=True)]

# Convert NaN values to empty string
df['Categories'] = df['Categories'].fillna('Unknown')

# Convert NaN values to empty string
df['Genres'] = df['Genres'].fillna('Unknown')

# Convert NaN values to empty string
df['Tags'] = df['Tags'].fillna('Unknown')

# Data summary and final checks
# -----------------------------------------------------

# # Set display options to show all rows and columns
# pd.set_option('display.max_rows', None)
# pd.set_option('display.max_columns', None)

# Check data types
print(df.dtypes)

# Summary statistics
print(df.describe())

# Check for missing values
print(df.isnull().sum())

# Frequency distribution and visualization
# -----------------------------------------------------

# Count the amount of games for each platform
windows_count = (df['Windows'] == 1).sum()
linux_count = (df['Linux'] == 1).sum()
mac_count = (df['Mac'] == 1).sum()

# Create a bar plot
plt.bar(['Windows', 'Linux', 'Mac'], [windows_count, linux_count, mac_count])
plt.title('Amount of games supporting operating system')
plt.xlabel('Operating System')
plt.ylabel('Count')
plt.show()

# Extract the year from the 'datetime' column
df['Year'] = df['Release date'].dt.year

# Group by year and calculate the maximum peak CCU
max_peak_ccu_by_year = df.groupby('Year')['Max Peak CCU'].max()

# Drop the intermediate 'Year' column if needed
df.drop('Year', axis=1, inplace=True)

# Create a line plot for max peak CCU by year
plt.plot(max_peak_ccu_by_year.index, max_peak_ccu_by_year.values, marker='o')

# Set plot title and axis labels
plt.title('Max Peak CCU by Year')
plt.xlabel('Year')
plt.ylabel('Max Peak CCU')

# Show the plot
plt.show()

# Specify the columns for frequency analysis
columns_to_analyze = ['Estimated owners', 'Price range', 'num_Publishers', 'num_Categories', 'Has_English',
                      'num_Genres', 'num_Tags', 'num_Supported languages', 'num_Full audio languages',
                      'num_Platforms', 'Content_Warning_Indicator', 'Release by Year and Quarter', 'Release by Quarter']

# Perform frequency analysis for each column
for column in columns_to_analyze:
    # Frequency analysis
    freq_counts = df[column].value_counts()
    print("Frequency Analysis -", column, ":\n", freq_counts, "\n")

    # Plot the frequency distribution
    plt.figure(figsize=(8, 6))
    sns.countplot(data=df, x=column)
    plt.title("Frequency Distribution - " + column)
    plt.xlabel(column)
    plt.ylabel("Frequency")
    plt.xticks(rotation=45)
    plt.show()



# # Save to reduced csv
# df.to_csv('games_reduced.csv', index=False)

# Manual one hot encoding of categories, genres and tags
# -----------------------------------------------------

# df = pd.read_csv('games_reduced.csv')

#Todo take only top (20) tags, categories and genres and the rest put in other
# Check using frequency analysis.
# check for each category\tags\genres\developers\publishers the number of peak CCU distribution using KDE and
# if its normal consider dropping.

def get_top_n_frequent(column, n=20):
    unique_values = set()
    for value in df[column]:
        if isinstance(value, str):
            unique_values.update(value.split(','))

    value_counts = {}
    for value in unique_values:
        value_counts[value] = df[column].astype(str).str.contains(value, regex=False).sum()

    sorted_values = sorted(value_counts.items(), key=lambda x: x[1], reverse=True)
    top_n_values = dict(sorted_values[:n])

    # Calculate the total count of values not in the top n
    other_count = sum(value[1] for value in sorted_values[n:])

    return top_n_values, other_count


# Function to create a plot for a column
def create_bar_chart(column, n=20):
    # Get the top n most frequent values in the column
    top_values, other_count = get_top_n_frequent(column, n)

    # Add the "Other" category with its count
    top_values['Other'] = other_count

    # Plot the bar chart
    plt.figure(figsize=(12, 6))
    x = list(top_values.keys())
    y = list(top_values.values())
    x_pos = np.arange(len(x))

    # Create the bar plot
    bars = plt.bar(x_pos, y)
    plt.xlabel(column)
    plt.ylabel('Frequency')
    plt.title(f'Top {n} Most Frequent {column}')

    # Adjust the position and alignment of x-ticks
    plt.xticks(x_pos, x, rotation=45, ha='right')

    # Add labels on each bar
    for i, bar in enumerate(bars):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width() / 2, height, str(height), ha='center', va='bottom')

    plt.tight_layout()
    plt.show()

# Create plots for 'Categories, 'Tags', 'Genres', 'Developers', and 'Publishers'
columns = ['Publishers', 'Developers']
for column in columns:
    create_bar_chart(column)

    # Calculate the total count for the column
    total_count = len(set(','.join(df[column].astype(str)).split(',')))
    print(f"Total unique {column}: {total_count}")
    
# Model training and comparison
# -----------------------------------------------------

df = pd.read_csv('games_reduced.csv')

# Drop categorical features
categorical_features = ['Supported languages', 'Full audio languages','Developers', 'Publishers', 'Categories', 'Genres', 'Tags']

df = df.drop(columns=categorical_features)

# Todo Create dummies for quarter

# Todo remove date and year and quarter

# Splitting the dataset into features (X) and target variable (y)
X = df.drop(columns=['Peak CCU'])
y = df['Peak CCU']

# Splitting into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Preprocessing numeric features
numeric_features = ['Required age', 'Price_(usd)', 'Metacritic score', 'User score', 'Positive',
                    'Negative', 'Achievements', 'Recommendations', 'Average playtime forever',
                    'Average playtime two weeks', 'Median playtime forever', 'Median playtime two weeks',
                    'num_Publishers', 'num_Categories', 'num_Genres', 'num_Tags',
                    'num_Supported languages', 'num_Full audio languages', 'num_Platforms',
                    'Review Ratio', 'Price per Avg Playtime Forever', 'Price per Avg Playtime Two Weeks',
                    'Price per Median Playtime Forever', 'Price per Median Playtime Two Weeks',
                    'Content_Warning_Indicator']

# Todo outlier detection on numeric columns.

numeric_transformer = StandardScaler()

# Column Transformer for applying different preprocessing to different feature types
preprocessor = ColumnTransformer(
    transformers=[
        ('num', numeric_transformer, numeric_features),
    ])

# Applying preprocessing to the training set
X_train = preprocessor.fit_transform(X_train)

# Applying preprocessing to the testing set
X_test = preprocessor.transform(X_test)

# Initialize models
lr = LinearRegression(n_jobs=-1)
lasso = Lasso()
rf = RandomForestRegressor(n_jobs=-1, n_estimators=50, max_depth=3, min_samples_split=5)
gb = GradientBoostingClassifier(n_estimators=50, max_depth=3, min_samples_split=5)

# Train and evaluate Random Forest
rf.fit(X_train, y_train)
rf_pred = rf.predict(X_test)
rf_rsqr =
rf_mse =
rf_rmse =
rf_mae =
print("done rf")

# Train and evaluate Gradient Boosting
gb.fit(X_train, y_train)
print("done fit")
gb_pred = gb.predict(X_test)

print("done gb")

# Todo add Liner regression
# Todo add Lasso
# Todo Get feature importance from all models.

# Define the data as a list of lists
data = [
    ["Decision Tree", dt_accuracy, dt_precision, dt_recall, dt_f1],
    ["Random Forest", rf_accuracy, rf_precision, rf_recall, rf_f1],
    ["Gradient Boosting", gb_accuracy, gb_precision, gb_recall, gb_f1],
    ["Naive Bayes", nb_accuracy, nb_precision, nb_recall, nb_f1]
]

# Define the headers for the table
headers = ["Model", "Accuracy", "Precision", "Recall", "F1-score", "ROC-AUC"]

# Print the table
print(tabulate(data, headers=headers, tablefmt="grid"))

# Create the table plot
fig, ax = plt.subplots(figsize=(10, 4))
ax.axis('off')
table = ax.table(cellText=data, colLabels=headers, cellLoc='center', loc='center')

# Style the table
table.auto_set_font_size(False)
table.set_fontsize(12)
table.scale(1, 1.5)

# Display the plot
plt.show()

# Todo pca take it into 2 dimensions
# Todo K-means