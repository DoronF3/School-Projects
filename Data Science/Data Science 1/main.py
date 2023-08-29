import json

import numpy as np
import pandas as pd
import requests

# Urls for https requests
url_games = "https://free-nba.p.rapidapi.com/games"
url_stats = "https://free-nba.p.rapidapi.com/stats"

# Header
headers = {
    "X-RapidAPI-Host": "free-nba.p.rapidapi.com",
    "X-RapidAPI-Key": '0256dec761msh8006183e7017986p13f234jsn57d3240777fb'
}

# Querystring
querystring = {"page": 348, "per_page": "25"}

# Download Games data
games_data = []
response_games = requests.get(url_games, headers=headers, params=querystring)

# Convert JSON to CSV
games_json = json.loads(response_games.text)
games_normalized = pd.json_normalize(games_json['data'])
games_data.append(games_normalized)
games_df = pd.concat(games_data, ignore_index=True)
games_df.to_csv('game_data_all.csv', index=False)
games_df.sort_values(['id'])

# Download Stats data
stats_data = []
querystring = {"page": 10, "per_page": "25"}
response_stats = requests.get(url_stats, headers=headers, params=querystring)

# Convert JSON to CSV
stats_json = json.loads(response_stats.text)
stats_normalized = pd.json_normalize(stats_json['data'])
stats_data.append(stats_normalized)
stats_df = pd.concat(stats_data, ignore_index=True)
stats_df.to_csv('stats_data_all.csv', index=False)
stats_df.sort_values(['game.id'])
print(stats_df.loc[:, 'game.id'])

# Merge dataframes
merged_df = stats_df.merge(games_df, how='left', right_on='id', left_on='game.id')

# Convert to CSV
merged_df.to_csv('merged_games_stats.csv', index=False)

# Load the merged games data into a pandas dataframe
merged_games_df = pd.read_csv('merged_games_data.csv')

# loop through all columns in the dataframe
for col in merged_games_df.columns:
    # check if the column is player since its empty
    if col == 'player':
        # fill the missing values with 'Unknown'
        merged_games_df[col].fillna('Unknown', inplace=True)
        # check if the column type is float64
    elif merged_games_df[col].dtype == 'float64':
        # fill the missing values with the mean of the column
        merged_games_df[col].fillna(merged_games_df[col].mean(), inplace=True)
    else:
        # fill the missing values with 'Unknown'
        merged_games_df[col].fillna('Unknown', inplace=True)

# Save the imputed data to a new csv file
merged_games_df.to_csv('merged_games_data_imputed.csv', index=False)

# Load the imputed games data into a pandas dataframe
merged_games_df = pd.read_csv('merged_games_data_imputed.csv')

# Filter by season
merged_games_df = merged_games_df[merged_games_df['season'] >= 2019]

# Save the imputed data to a new csv file
merged_games_df.to_csv('merged_games_data_2019.csv', index=False)

# Load the over 2019 games data into a pandas dataframe
merged_games_df = pd.read_csv('merged_games_data_2019.csv')

# Sort by game_id and date
merged_games_df = merged_games_df.sort_values(['game_id', 'date'])

# Save the imputed data to a new csv file
merged_games_df.to_csv('merged_games_data_sort.csv', index=False)

# Load the sort games data into a pandas dataframe
merged_games_df = pd.read_csv('merged_games_data_sort.csv')

# Create a dictionary of column names to be replaced
replace_dict = {
    "ast": "player_assists",
    "blk": "player_blocks",
    "dreb": "player_defensive_rebounds",
    "fg3_pct": "player_three_point_pct",
    "fg3a": "player_three_point_attempts",
    "fg3m": "player_three_point_made",
    "fg_pct": "player_field_goal_pct",
    "fga": "player_field_goal_attempts",
    "fgm": "player_field_goal_made",
    "ft_pct": "player_free_throw_pct",
    "fta": "player_free_throw_attempts",
    "ftm": "player_free_throw_made",
    "min": "player_minutes_played",
    "oreb": "player_offensive_rebounds",
    "pf": "player_personal_fouls",
    "pts": "player_points",
    "reb": "player_total_rebounds",
    "stl": "player_steals"
}

# Rename the columns
merged_games_df = merged_games_df.rename(columns=replace_dict)

# Save the imputed data to a new csv file
merged_games_df.to_csv('merged_games_data_renamed.csv', index=False)

# Load the renamed games data into a pandas dataframe
merged_games_df = pd.read_csv('merged_games_data_renamed.csv')

# Create new features
merged_games_df['team_won'] = merged_games_df.apply(lambda x: x['home_team_score'] > x['visitor_team_score'] and x['team_id'] == x['game_home_team_id'], axis=1)
merged_games_df['abs_score_difference'] = abs(merged_games_df['home_team_score'] - merged_games_df['visitor_team_score'])
merged_games_df['player_full_name'] = merged_games_df['player_first_name'] + ' ' + merged_games_df['player_last_name']
merged_games_df = merged_games_df.drop(['player_first_name', 'player_last_name'], axis=1)
merged_games_df['player_height_cm'] = merged_games_df['player_height_feet'] * 30.48 + merged_games_df['player_height_inches'] * 2.54
merged_games_df['home_player_stats'] = merged_games_df.apply(lambda x: x['team_id'] == x['game_home_team_id'], axis=1)

# Create new feature: 'player_efficiency'
merged_games_df['player_efficiency'] = (merged_games_df['player_points'] + merged_games_df['player_total_rebounds'] +
                                        merged_games_df['player_assists'] + merged_games_df['player_steals'] + merged_games_df['player_blocks'] -
                                        (merged_games_df['player_field_goal_attempts'] - merged_games_df['player_field_goal_made']) -
                                        (merged_games_df['player_free_throw_attempts'] - merged_games_df['player_free_throw_made']) -
                                        merged_games_df['turnover'])

merged_games_df['player_three_to_throw_attempt_ratio'] = merged_games_df['player_three_point_attempts'] / merged_games_df['player_field_goal_attempts']
merged_games_df['player_three_to_throw_made_ratio'] = merged_games_df['player_three_point_made'] / merged_games_df['player_field_goal_made']

merged_games_df.to_csv('merged_games_data_features.csv', index=False)

# Load the features games data into a pandas dataframe
merged_games_df = pd.read_csv('merged_games_data_features.csv')

# Remove redundant and irrelevant columns
merged_games_df.drop(['Unnamed: 0', 'id_x', 'id_y', 'game_status', 'player_height_feet', 'player_height_inches',
                      'team_abbreviation', 'team_conference', 'team_city', 'team_name', 'player', 'status', 'home_team_abbreviation',
                      'home_team_city', 'home_team_name', 'visitor_team_abbreviation', 'visitor_team_city', 'date',
                      'home_team_score', 'period', 'postseason', 'season', 'time', 'visitor_team_score', 'home_team_id',
                      'visitor_team_id', 'visitor_team_name'],
                     axis=1, inplace=True)

merged_games_df.to_csv('nba_data_preporcessed.csv', index=False)

# Load the preprocessed games data into a pandas dataframe
games_df = pd.read_csv('nba_data_preporcessed.csv')

# Filter only the 2019 games
games_df = games_df[games_df['game_season'] == 2019]

# Create an empty dictionary to store team wins count
team_wins = {}

# Iterate over each game_id
for game_id in games_df['game_id'].unique():
    # Get the home team and visitor team names for the current game_id
    home_team = games_df.loc[games_df['game_id'] == game_id, 'home_team_full_name'].values[0]
    visitor_team = games_df.loc[games_df['game_id'] == game_id, 'visitor_team_full_name'].values[0]

    # Initialize the count of wins for each team if not present in the dictionary
    if home_team not in team_wins:
        team_wins[home_team] = 0
    if visitor_team not in team_wins:
        team_wins[visitor_team] = 0

    # Determine the winner based on the abs_score_difference
    abs_score_difference = games_df.loc[games_df['game_id'] == game_id, 'abs_score_difference'].values[0]
    if abs_score_difference > 0:
        team_wins[home_team] += 1
    else:
        team_wins[visitor_team] += 1

# Find the team with the maximum and minimum wins
team_most_wins = max(team_wins, key=team_wins.get)
team_least_wins = min(team_wins, key=team_wins.get)

# Print the team names with maximum and minimum wins
print(f"The team with the most wins is: {team_most_wins, team_wins[team_most_wins]}")
print(f"The team with the least wins is: {team_least_wins, team_wins[team_least_wins]}")


# Initialize win_points and lose_points variables
win_points = 0
lose_points = 0

# Iterate over each game_id
for game_id in games_df['game_id'].unique():
    # Get the home team and visitor team names for the current game_id
    home_team = games_df.loc[games_df['game_id'] == game_id, 'home_team_full_name'].values[0]
    visitor_team = games_df.loc[games_df['game_id'] == game_id, 'visitor_team_full_name'].values[0]

    # Get the home team score and visitor team score for the current game_id
    home_team_score = games_df.loc[games_df['game_id'] == game_id, 'game_home_team_score'].values[0]
    visitor_team_score = games_df.loc[games_df['game_id'] == game_id, 'game_visitor_team_score'].values[0]

    # Check if the home or visitor team is Los Angeles Lakers or Charlotte Hornets
    if home_team == team_most_wins or visitor_team == team_most_wins:
        win_points += home_team_score if home_team == team_most_wins else visitor_team_score
    if home_team == team_least_wins or visitor_team == team_least_wins:
        lose_points += home_team_score if home_team == team_least_wins else visitor_team_score

# Print the total win points and lose points
print("Total win points:", win_points)
print("Total lose points:", lose_points)
