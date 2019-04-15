package Subroutines;

import com.mysql.jdbc.Connection;
import com.mysql.jdbc.Statement;
import com.rivescript.macro.Subroutine;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 *
 * @author Aswin van Woudenberg
 */
public class JdbcSubroutine implements Subroutine {

    public String call(com.rivescript.RiveScript rs, String[] args) {
        String sql = "";
        String result = "";
        for (int i=0; i<args.length; i++)
            sql = sql + " " + args[i];
        sql = sql.trim();


        Connection connection = null;
        Statement statement = null;
        ResultSet resultSet = null;

        try {
            connection=(Connection) DriverManager.getConnection(
                    "jdbc:mysql://127.0.0.1:3306/imdb", "root" , "");
            statement=(Statement) connection.createStatement();
            resultSet=statement.executeQuery(sql);
            while(resultSet.next()) {
                int i = resultSet.getMetaData().getColumnCount();
                for (int j = 1; j <= i; j++) {
                    if (result.equals("")) {
                        result = resultSet.getString(j);
                    } else {
                        result += resultSet.getString(j) + " ";
                    }
                }
                if (!result.equals("")) {
                    result += "\n";}
                if (result.length() > 1800) {
                    result += "and more.";
                    break;
                }
            }
        } catch (SQLException ex) {
        } finally{
            try {
                resultSet.close();
                statement.close();
                connection.close();
            } catch (SQLException ex) {
            }
        }
        return result;
    }
    
}
