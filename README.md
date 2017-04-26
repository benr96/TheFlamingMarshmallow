# The Flaming Marshmallow

# ---MAIN GAME IS LOCATED IN TESTINGBRANCH---

## INTRODUCTION

A group project for DT228/2 OOP's Assignment 3 by Something Developers which includes Ben Ryan, Yit Chee Chin, Dylan Kirby and Patrick Malone. For this assignment we decided to make a game. We used Unreal Engine 4 as a development platform and did all programming in C++. All models used in the game are original and created using Blender. All sound effects are also original.

## GAME

Endless survival horror between a lone marshmallow and a horde of chocolate enemies. The only goal is to survive. You must fight against the horde using any tools at your disposal which includes a targeting system that can be used to attack the enemies, various items scattered across the map to either revive health or boost your stats and a special flame attack that vastly increases your attacking power for a brief amount of time. Enemies will quickly surround you if you show even a hint of mercy, so eradicate the chocolate foes with brutal vengence.

## CONTROLS

- W - Forward
- A - LEFT
- S - DOWN
- D - RIGHT
- Space - Jump
- Double space - Double Jump
- Shift - Run

- F - Activate Flames
- I - Opens/Closes Inventory
- P - Pause
- X - Pick up item
- Double tap Direction - Dash

- TAB - Toggles Target Enemy (When in range)
- LEFT CLICK - Attacks Enemy (When targeted)

## FEATURES

- AI - The AI is made by utilizing a Behavior Tree which runs from code from the Enemy character class and the Enemy Controller. The Enemy AI works in two ways, by patrolling and then following the player if they have been targeted. For the patrol the AI begins by finding waypoints placed around the map, picking one of these waypoints at random and then moving towards it. The enemy character in the meantime has a Pawn Sensing Component, which gives the enemy the ability to "see" the marshmallow. When this happens the enemy is sent to attack the marshmallow and follows the player. Also as part of the AI, the enemy can do damage to the marshmallow, it can be targeted by allowing each enemy to be entered into an array and they can knockback the marshmallow slightly.

- Inventory - The inventory can be accessed using the I key on the keyboard. Once pressed a menu will open up displaying a number of boxes which indicate a spot an item can be placed in. Up to 30 items can be stored and each item can be either used or dropped once left clicked. There are 3 types of items that are scattered accross the map, each colour coded to make them easily indentifiable. The red apple can be used to restore 20 health back to the marshmallow. The Blue apple gives a 20% speed boost for 20 seconds and similarily, the green apple gives a 20% base damage boost.

- HUD - The HUD displays to the player two abilities on offer, (dash and flames) aswell as the Health of the marshmallow. The Dash is an ability for the marshmallow to quikly dash in a certain direction, which then recharges over a short period of time to use again later. The flames ability gives the marshmallow a damage boost, and is used all at once. It then recharges and can only be used when it is fully recharged. Health can be reduced by damage taken frmo enemies, but can also be restored by pickups. 




## THE STORY

Mallow, a young sweet from the marshmallow kingdom has lived his life knowing nothing but war. A long arduous war invloving the four great kingdoms of Candyland; Chocolate, Marshmallow, Ice and Jellies. Living on the border between the Marshmallow and Chocolate kingdom, Mallow's village had always expected the day that the chocolate would invade. They were a peaceful village that 
wanted nothing but to see their children grow up and see a world where they could wonder without worry of conflict and strife. Mallow was a happy child that grew up in a caring family. While the war always made him feel anxious, his optimism and abiltiy to see positivity in everything kept him smiling through these tough times. The one thing he always wondered was why the two kingdoms couldn't be friends.

The Chocolate kingdom was vast in comparison to the rest of the continent and it was rapidly furthering their reach. The ice kingdom had already been reduced to nearly nothing after the Chocolate's managed to drain their land of their sweetness, thus turning it into a scorching place of heat, melting the kingdom to it's knees. The Chocolate had gained access to a mysterious piece of technology that overwhelmed the kingdom assuring there was nothing the Ice could do to resist. All those who were not melted were taken prisoner, but nevertheless the kingdom had fallen a decade before the present day.

At around the same time as the melting incident, the Chocolate kingdom also looked to gain more land elsewhere, thus gaining more reasorces for their already unstoppable army. And being on the border between the two strifing kingdoms, it was Mallow's village that was to take the hit. The chocolate's were quick and ruthless savages, eating everything in their wake. The villagers had little combat experience so they could only put up so much of a fight before they succumbed to their fate. Mallow's father, being the lookout of the village had anticapated this surprise attack before anyone else could and with this chance, he did the only thing a loving father would in this situation. He did the very best to ensure the safety of his child Mallow.

Mallow was but a child and yet he was sent off on his own outside the safety of his village for the first time. Unsure of the fate his parents and everyone he had ever knew or cared about held, Mallow began to grow a resentment towards the kingdom of Chocolate, the ones who were propagating and prolonging the needless war in mallows eyes and in this rage and resentment, Mallow swore to take 
revenge on the Chocolate kingdom and end this war once and for all. Armed with the power of being able to burst into flames at will, mallow holds a crucial advantages that could swing the tides of power between the two kingdoms.

This game is but a snapshot of the trials that mallow must make.



