# CPPBC_Vapor
C++BC Game Publishing Platform

This is a simple platform operated by <b>Users</b>, which come in two varieties:
<ul>
  <li><b>AdminUsers</b> have the highest privileges, being able to <b>view all users</b> on the system, <b>add and remove users</b>, <b>add and remove games</b> and <b>query user information</b>. On top of this, they can also <b>view collected data</b> about other users, such as <b>transaction logs</b> and <b>play history logs</b>.</li>
 
<li><b>PlayerUsers</b> each maintain a <b>wallet</b> of funds and a <b>bag</b> of games that they own. The UI provides access to the store where players can <b>buy games</b> that are currently in the store and <b>play games</b> from their bag. If they so wish, players can also <b>remove games</b> from their accounts. The system logs every play session that the PlayerUser has and stores it in the database.</li>
</ul>

Every <b>user</b> is identified by a unique <b>username</b> and a <b>password</b> that they use to log into the platform. Each user also has an email address that must contain one and only '@' symbol.

Every <b>Game</b> is identified by a unique <b>ID number</b> and features a </b>title</b>, a brief <b>description</b> and a <b>price</b>. 

At start up, the user is presented with a menu, where they can:
1. View All Games (outputs a list of all games to the screen)
2. Login<br>
Q. Quit (this closes the program)

Should an AdminUser log in, they will be presented with the option to:

