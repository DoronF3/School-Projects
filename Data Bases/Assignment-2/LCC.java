import java.sql.*;

public class LCC {
    public static void executeLCC() {
        // Initialize query strings
        String readUsersQuery = null;
        String addColumnQuery = null;
        String readFriendsQuery = null;
        String countMutualQuery = null;
        String updateQuery = null;

        try {
            // Load PostgreSQL driver
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            return;
        }

        Connection connection = null;
        try {
            // Establish database connection
            connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/socialnetwork", "vagrant", "vagrant");
        } catch (SQLException e) {
            e.printStackTrace();
            return;
        }

        if (connection != null) {
            try {
                Statement stmt = connection.createStatement();

                // Read users table
                readUsersQuery = "SELECT * FROM users";
                ResultSet rs1 = stmt.executeQuery(readUsersQuery);

                // Add llc column to users table if not exists
                addColumnQuery = "ALTER TABLE users ADD COLUMN IF NOT EXISTS llc REAL";
                stmt.execute(addColumnQuery);

                while (rs1.next()) {
					// Get the current user id
                    String userId = rs1.getString("userid");
                    
                    // Read friends of the current user
                    readFriendsQuery = "SELECT * FROM friends WHERE userid1 = ?";
                    PreparedStatement friendsStatement = connection.prepareStatement(readFriendsQuery);
                    friendsStatement.setString(1, userId);
                    ResultSet rs2 = friendsStatement.executeQuery();
                    
                    int count = 0;
                    int pairs = 0;

                    while (rs2.next()) {
						// Get the current friend username
                        String friendUsername = rs2.getString("userid2");
                        
                        // Prepare query to count mutual friends
                        countMutualQuery = "SELECT COUNT(*) AS mutual_friends FROM friends WHERE userid1 = ? AND userid2 IN (SELECT userid2 FROM friends WHERE userid1 = ?)";
                        PreparedStatement countStatement = connection.prepareStatement(countMutualQuery);
                        countStatement.setString(1, friendUsername);
                        countStatement.setString(2, userId);
                        ResultSet rs3 = countStatement.executeQuery();

                        if (rs3.next()) {
                            int mutualFriends = rs3.getInt("mutual_friends");
                            pairs += mutualFriends;
                        }

                        count++;
                    }

                    double llc = (count > 1) ? (2.0 * pairs) / (count * (count - 1)) : 0.0;
                    
                    // Prepare query to update llc value for the current user
                    updateQuery = "UPDATE users SET llc = ? WHERE userid = ?";
                    PreparedStatement updateStatement = connection.prepareStatement(updateQuery);
                    updateStatement.setDouble(1, llc);
                    updateStatement.setString(2, userId);
                    updateStatement.executeUpdate();
                    updateStatement.close();
                }

                connection.commit();
                stmt.close();
                connection.close();

                System.out.println("LCC computation and update completed.");

            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] argv) {
        executeLCC();
    }
}
