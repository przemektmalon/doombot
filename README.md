# doombot
AI bot for Doom written in C

# This program:

Parses .WAD files where doom maps are stored.

Builds a list of traversal nodes in preparation for use in a* pathfinding algorithm.

# Will soon:

Use the a* pathfinding algorithm to find an optimal path through a level.

Send GET signals to the 'rest' API to query enemy and item positions.

Use pathfinding and queried game state to decide on player actions.

Send POST signals to the 'rest' API within doom (embedded JSON and HTML within the chocolate doom engine) to control the player.

Complete doom levels.

# Progress

An example of a parsed map file with traversal nodes and their connections rendered from an svg.

![Example](https://github.com/przemektmalon/doombot/blob/master/img/traversal2.png)
