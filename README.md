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
1. List All Games (outputs a list of all games to the screen)
2. Login (requires username and password)
3. (Q) Quit (this closes the program)

Should an AdminUser log in, they will be presented with the option to press:
1. <b>List All Games</b> 
2. <b>List All Users</b> (displays all users to the screen in the format AUTHORITY | USERNAME | EMAIL | FUNDS)
3. <b>Add Game</b> (enters a game registration screen, adds a game to the system based on input)
4. <b>Remove Game</b> (gives option to delete any game from the system, gives option to replace the deleted game)
5. <b>Add User</b> (enters a user registration screen, adds user to the system based on input)
6. <b>Remove User</b> (gives option to delete any user from the system, except themselves)
7. <b>View Play Logs</b> (displays a chosen player's play history to the screen, including total and average play times)
8. <b>View Transactions</b> (displays a chosen player's transaction history to the screen)
9. <b>(Q) Logout</b> (goes back to the main menu)

If an AdminUser removes a game that is owned by one or more PlayerUsers, then:
<ul>
  <li>either the game is removed completely without replacement, in which case, each player is reimbursed the amount that they paid for that game,</li>
  <li>or the game is replaced, in which case, each affected player receives this game for free.</li>
</ul>

Should a PlayerUser log in, they will be shown their current balance and have the option to press:
1. <b>List All Games</b>
2. <b>List My Games</b>
3. <b>Buy Game</b> (enters a purchase screen, asks for a Game ID corresponding to the game they would like)
4. <b>Play Game </b>(asks for a Game ID that the player owns, 'loads' the game, then times and logs the session)
5. <b>Delete Game </b>(allows the player to remove a game that they own from their account only)
6. <b>Add Funds</b> (allows the player to add funds to their account)
7. <b>(Q) Logout</b>

# Storage
All of the associated data is stored persistently in ```.txt``` files within a ```data``` folder, which is split into:
<ul>
  <li>Separate files containing <b>all AdminUsers</b> and <b>all PlayerUsers</b> together.</li>
  <li>A file containing a <b>master list of games</b> available on the platform.</li>
  <li>A folder containing a <b>transaction log</b> file for each PlayerUser.</li>
  <li>A folder containing a <b>play history</b> file for each PlayerUser.</li>
  <li>Each PlayerUser also has a bag file listing the IDs of the <b>games they own</b>.</li>
</ul>

At run time, whenever records change (users removed/added, games bought/removed/added, etc.) these files, as well as the running platform, are updated to record this on the fly. When the program loads up again, it reads from these modified files so that the state of the system persists between closing and starting. 
