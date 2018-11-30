# CPPBC_Vapor
C++BC Game Publishing Platform

This is a simple platform operated by <b>Users</b>, which come in two varieties:
<ul>
  <li><b>AdminUsers</b> have the highest privileges, being able to <b>view all users</b> on the system, <b>add and remove users</b>, <b>add and remove games</b> and <b>query user information</b>. On top of this, they can also <b>view collected data</b> about other users, such as <b>transaction logs</b> and <b>play history logs</b>.</li>
 
<li><b>PlayerUsers</b> each maintain a <b>wallet</b> of funds and a <b>bag</b> of games that they own. The UI provides access to the store where players can <b>buy games</b> that are currently in the store and <b>play games</b> from their bag. If they so wish, players can also <b>remove games</b> from their accounts. The system logs every play session that the PlayerUser has and stores it in the database.</li>
</ul>

Every <b>user</b> is identified by a unique <b>username</b> and a <b>password</b> that they use to log into the platform. Each user also has an email address that must contain one and only '@' symbol.

Every <b>Game</b> is identified by a unique <b>ID number</b> and features a </b>title</b>, a brief <b>description</b> and a <b>price</b>. 

At start up, the user is presented with a menu, where they can press:
(1) List All Games (outputs a list of all games to the screen)
(2) Login (requires username and password)
(3) Quit (this closes the program)

Should an AdminUser log in, they will be presented with the option to press:
(1) List All Games 
(2) List All Users (displays all users to the screen in the format AUTHORITY|USERNAME|EMAIL|FUNDS)
(3) Add Game (enters a game registration screen, adds a game to the system based on input)
(4) Remove Game (gives option to delete any game from the system, gives option to replace the deleted game)
(5) Add User (enters a user registration screen, adds user to the system based on input)
(6) Remove User (gives option to delete any user from the system, except themselves)
(7) View Play Logs (displays a chosen player's play history to the screen)
(8) View Transactions (displays a chosen player's transaction history to the screen)
(q) Logout (goes back to the main menu)

If an AdminUser removes a game that is owned by one or more PlayerUsers, then:
<ul>
  <li>either the game is removed completely without replacement, in which case, each player is reimbursed the amount that they paid for that game,</li>
  <li>or the game is replaced, in which case, each affected player receives this game for free.<\li>
</ul>

Should a PlayerUser log in, they will be shown their current balance and have the option to press:
(1) List All Games
(2) List My Games
(3) Buy Game (enters a purchase screen, asks for a Game ID corresponding to the game they would like)
(4) Play Game (asks for a Game ID that the player owns, 'loads' the game, then times and logs the session)
(5) Delete Game (allows the player to remove a game that they own from their account only)
(6) Add Funds (allows the player to add funds to their account)
(q) Logout
