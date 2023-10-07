# INS: Instructions



N->Length of the sequence.

M->Number of symbols

D->Time in seconds for which every symbol of the generated sequence is shown.

T->Time in seconds. Indicates the maximum time you must take to input every symbol.





MAIN MENU-> as the game starts the main menu is displayed. It contains 5 options: Practice, Story, TOP Scores, Shop, Initialize. 

	You can press the LEFT or RIGHT buttons to switch from one menu option to another.

	At the bottom right you can see the coins that you have earned so far by playing the story mode.



Initialize (Main menu option)-> resets the high scores, the coins and the owned colors. You should run the initliaze operation the first time you run the game.

Practice (Main menu option)-> As you pass to the practice mode by pressing SELECT BUTTON, a menu is displayed where you can set the sequence Length (N), number of Symbols(M), and D.

	The GO Back option allows you to return to the main menu.

Playing Game-> refers to when you start playing in story mode or in practice mode. First a sequence is generated and is shown. The sequence length is N and contains up to M

	symbols (chosen from a subset of right, left, up and down arrows). Every symbol will be displayed with a delay of up to D seconds (exactly D in practice mode).

	 Once the sequence is shown you can see a timer starts ticking at the bottom right. You must type every character within the timer shown at the bottom right.

Game outcome-> When a gameplay ends you can see your inputs on the top row of the display and the generated sequence is shown on the second row for 1 to 2 seconds. 

	If you win the backlight flashes setting the background to green. If you lose the backlight becomes red and you can extend the time for which the sequences are

	shown by keeping pressed the SELECT button.

Story (Main menu option)-> When you select the story mode you are shown a menu where you can set the initial values of N, M, D and T. 

	The GO back option allows you return to the main menu.

TOP scores (Main menu option)-> when selected, displays the list of top 5 scorers with their scores and aliases.

Shop (Main menu option)-> contains 8 backlight colours and you can buy your favourite colours and set them as backlight.

Rewards (Main menu option)-> contains the rewards that you win as you go on playing the story mode game.







# MFS: Features of the minimal feature set



Indicate by deleting YES or NO to the questions below.

If partially implemented, describe this by deleting *YES/NO* and

writing what does work.  Do not adjust any other text in this section.



* Sequences are displayed on the screen as specified: YES

* Backlight flashes (only when correct): YES

* Incorrect answer leads to game over: YES

* Successive stages get more difficult: YES

* Game supports *N* up to 10: YES

* Game supports *M* of size 4: YES

* A menu is present: YES

* Size of *M* can be changed: YES

* *N* can be changed: YES

* Message when wrong printed: YES



# OAF: Optional and additional features





* additional feature: at the end of each play showing the generated and the input sequence to the user.

* Handling sequences N that are longer than 10: sequences up to the length of 16 have been can be generated. This is because

after each play we show the user input and the generated sequence to the user and the LCD screen has a maximum limit of 16 symbols.

* time limit T for inputting the next character (by default 2 seconds). If a typing in takes longer than T,

the user fails. The value of T can be changed from the menu system

* T is displayed as a countdown, counting down as steps of 0.1 seconds.

* A story mode to build up through increasing N and M and reducing D. By default starting with N=4 and M=2 and moving up to N=8 before moving to

N=4 and M=3 etc.

* A menu to switch from practice mode to story mode (additional menu options: TOP scores, Shop, Rewards, Initialize)

* Selectable difficulty levels that set the start values for M, N, T and D, in

story mode.

*  I have defined my own graphical representation for displaying the generated symbols.

* Each of the symbols is defined as 2x2 matrix of lcd characters.

* Suitable displays for winning and losing have been defined: when a user wins a green display is shown and when loses a red display is shown.

* A high score table with top 5 scores is written to the EEPROM and there is an additional initial menu option to display the table

of high scores. The option is "TOP Scores".

* the winner of the top 5 high scores can give his alias that will be displayed in the high score table

* Additional feature: when you lose and the red display is shown with your inputs on the top row and the generated sequence in the second row for two seconds. 

If you keep pressed the SELECT button the delay is extended to until you leave the button.

* Additional feature: Initialize option in the main menu allows you to reset the values of the game (high scores, aliases and coins) to the start.

* Additional feature: coins and the shop: when a game in story mode ends you earn some coins, proportional to what you have scored in the game. Those coins

can be used in the shop, which is accessible from the main menu. The shop allows you to buy a new colour and set is as backlight. By default the backlight is dark. 

Each of the colours has a price. As you earn more coins you can buy and select your preferred colour/s

* Additional feature: as the total score scored by the user in story mode rises the user wins a reward emoji that can be seem from the erewards option in the main menu







# TPL: Test plan





1. Observe the main menu

2. (main menu) press the LEFT BUTTON (you should see the "Initialize" option, otherwise find it)

3. Press SELECT BUTTON

4. Press SELECT BUTTON again within 2 seconds. (Then the necessary values in EEPROM are initialized and the top score table is emptied)

5. Observe that at the bottom right, the coins value appears as "$0". (if "$0" does not appear you were probably too late, so go back to point 2)

6. (main menu) Using RIGHT/LEFT BUTTONS go to the "Practice" option and press SELECT BUTTON

7. A menu is displayed. Using RIGHT, LEFT and UP DOWN BUTTONS you can set the values of M,N and D or you can go back to the main menu.

8. Press SELECT BUTTON to play the game (if the "Go back option is not selected")

9. The game starts, so carefully watch the symbols (right, left, up and down) displayed as 2x2 matrix on the left.

10. As soon as the sequence ends, a timer starts ticking at the bottom right

11. start typing the sequence, as you remember it.

12. each of your input symbols is shown on the top row of the display as you type it.

13. Once you have entered all the symbols or the timer has reached 0, the generated sequence is shown again on the second row.

14. If you won the backlight flashes setting the background to green colour or to red if you lose. If you lose you can extend the time for which the symbols 

	are shown by keeping pressed the select button, so you can know where exactly you made the mistake.

15. Once a practice game ends you see the same display that you see at point 7.

16. Using RIGHT/LEFT BUTTONS, go to "GO Back" option and press SELECT BUTTON

17. You are back to the main menu

18. (main menu) Press RIGHT BUTTON (You should see "Story" option, otherwise find it) and press SELECT BUTTON to play the story mode.

19. You should see a similar menu to point 7. You can change the initial values of M, N, D and T or leave them as default

20. Press SELECT BUTTON to play (if "GO back" option is not selected)

21. The game has started so carefully watch the sequence and follow the steps 9 to 14 (then go to point 22 instead of 15)

22. If you win, you pass to the next level with increasing difficulty and repeat the step 21.

23. If you lose, the game over message is shown with your score. And if you have scored in top 5 you can type your alias (using UP, DOWN, RIGHT and LEFT keys) and save them.

	you might also win a secret emoji award.

24. Once the game is over, you reach the main menu and you should notice that the amount of coins in the bottom right has increased (if you scored more than 0)

25. (main menu) Press RIGHT BUTTON (you should see the "TOP score" option, otherwise find it) and press SELECT BUTTON to see top scores list.

26. You can see the top 5 scores here and each score is shown for 1 second with the alias of the player.

27. Press SELECT BUTTON to go back to the main menu.

28. (main menu) Press the RIGHT button (you should see the "shop" option, otherwise find it) and press SELECT to go to the shop.

29. Here you can find a list of backlight colours. Press up/down to discover the colours and their prices. If you don't have enough money, play more story mode games

	and earn more coins to buy your preferred colour.

30. Once you have bought your favourite colour (if you had the money) press RIGHT button ("Go back" Option will be displayed), press SELECT to go to the main menu.

31. (main menu) Press the RIGHT button (you should see the "REWARDS" option, otherwise find it) and press SELECT.

32. Press UP DOWN button to see all the rewards that you have won so far. If you have no rewards, play the story mode game for a few more times. Press SELECT to exit.

