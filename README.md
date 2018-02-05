# Assignment 3 - Zoo Tycoon
**Due by 11:59pm on Monday, 2/19/2018**

**Demos due by 11:59pm on Monday 3/5/2018**

The goal of this assignment is to start working with inheritance in C++.  Follow the directions below, and make sure your source code files (**no executable files, `.o` files, or other crufty stuff**) are committed and pushed back into your repository on GitHub to earn credit for the assignment.

## The Zoo Tycoon game

For this assignment, you will implement a small game where the player acts as the owner of the city's zoo, which has exhibits of monkeys, sea otters, and sloths.  As the owner, it's the player's job both to ensure the welfare of the animals and to generate as much profit as possible.  They accomplish these goals by investing in animals, feeding them, caring for them when they are sick, and raising their babies when they are born.

Each species of animal has these specific traits:
  * **Age**
    * For all species, an animal is an adult if it is at least 3 years old.
    * For all species, an animal is a baby if it is less than 30 days old.
  * **Cost**
    * Monkeys cost $15,000 each.
    * Sea otters cost $5,000 each.
    * Sloths cost $2,000 each.
  * **Babies**
    * Monkeys have 1 baby at a time.
    * Sea otters have 2 babies at a time.
    * Sloths have 3 babies at a time.
  * **Food cost**
    * The base daily food cost varies on a day-to-day basis (see below).
    * Monkeys have a daily food cost of 4 times the base food cost.
    * Sea otters have a daily food cost of 2 times the base food cost.
    * Sloths have a daily food cost equal to the base food cost.
  * **Revenue**
    * Each animal generates daily revenue equal to a percentage of the initial cost of one of its species.
      * All animals except monkeys generate 5% of the cost of one of their species (i.e. each sea otter generates $250 each day, and each sloth generates $100 each day).
      * Each monkey generates 10% of the cost of a monkey (i.e. each monkey generates $1500 each day).
    * On certain days, when attendance is high, each monkey generates some amount of bonus revenue.  See below for details.

## Game flow

The game starts with the owner having no animals and $100,000 in the bank, and it proceeds one day at a time.  You can think of each day as a turn for the player.  During a single day, several things happen:

  1. The age of each animal currently in the zoo increases by one day.

  2. The owner may buy up to two adult animals of a single species.  The owner may only buy one species per day, but they do not have to buy any animals if they don't want to.  Each animal that the owner buys is exactly 3 years old.  When the owner buys an animal, the cost of the animal is subtracted from the owner's bank account.

  3. The owner must pay the feeding cost for each animal in the zoo (including any they just bought).  The cost of food for each animal is calculated using the base cost of food.  This starts out as $50.  Each day, the base cost changes to a random value between 75% and 125% of the base cost from the day before.  Once the cost of food for each animal is calculated, this amount is subtracted from the owner's bank account.

  4. A special event occurs.  The special event is chosen at random from among the following:
    * One randomly chosen animal gets sick.  In order to care for the sick animal, the owner must pay an amount equal to half the initial cost of an animal of the same species as the sick animal (e.g. a sick monkey costs half of $15,000, i.e. $7,500).  If the owner has enough money to cover this cost, it is subtracted from their bank account.  If they do not have enough money, then the sick animal dies and is removed from the zoo.

    * A randomly chosen adult animal gives birth to the appropriate number of babies for its species (a non-adult can't have babies).  Each baby starts with age 0 and is added into the zoo.

    * A boom in zoo attendance occurs.  Each time this happens, each monkey generates a random amount of extra bonus revenue between $250 and $500.

    * No special event occurs on this day.

  5. The owner receives daily revenue for each animal, as specified above.

In addition to the specifications above, your game must have these features:
  * Each baby animal less than 30 days old generates twice the amount of revenue as an adult animal.  These babies who are less than 30 days old also cost twice as much as an adult if they get sick.

  * If the player runs out of money at any point, the game ends with the zoo going bankrupt.

## Program requirements

* You must have a class for each of the following things: zoo, animal, monkey, sea otter, and sloth.

* You must use inheritance: the classes for monkey, sea otter, and sloth must inherit some traits and behaviors from the animal class, though each of them may also have unique traits or behaviors, as well (e.g. the bonus payout for monkeys).

* Within your zoo, the exhibit of each species of animal must be represented as a dynamically-allocated array of objects of the appropriate class.

* Your program should implement the game flow described above.  The player may play one day at a time until they choose to quit the came.  At the beginning of each day, you should let the player know how much money they have in the bank and how many adults (>= 3 years old) and babies (< 30 days old) they have of each species.

* Your program may not have any memory leaks.

* Your program must be factored into interface, implementation, and application.  Specifically, you should have one header file and one implementation file for each class, and you should have a single application file containing your `main()` function.  You should also write a makefile that specifies compilation for your program.

## Extra credit

In addition to the requirements above, you may earn extra credit as follows:

  * (5 points) Implement a new animal class using inheritance.  The new animal must include all of the traits of the other animals (age, cost, babies, food cost, revenue).

  * (5 points) Allow the user to choose a type of feed each day:
    * **Regular** - This is the normal food, and it behaves as described above.
    * **Premium** - This food costs twice as much as regular food for all animals, but it reduces the probability of sickness by half.
    * **Cheap** - This food costs half as much as regular food for all animals, but it doubles the probability that an animal will get sick.

## Code style

You must include a header comment for each source code file that contains a description of the file (including how to run the program, command line arguments, etc. if the file contains your `main()` function), your name, and the date.  Your code should be well commented, including header comments for *all* functions describing what the function does, its parameters, and any pre- and post-conditions for the function.  You should appropriately use whitespace, newlines, and indentation.

Make sure you review the style guidelines for the course, and start trying to follow them:

http://web.engr.oregonstate.edu/~hessro/static/media/162_style_guideline.4812c1d9.pdf

## Submitting your program

To submit your program, you need to make sure the following files are committed into your git repository and pushed to your `master` branch on GitHub before the due date above:
  * The `.cpp` file containing your application code.
  * All of the `.hpp` and `.cpp` files containing the interface and implementation of your classes.
  * Your `Makefile`.

Do not commit any other files (other than the ones that were already in your repository at the start of the assignment).  A good way to check whether your assignment is submitted is to simply look at your repo on GitHub (i.e. https://github.com/OSU-CS162-W18/assignment-3-YourGitHubUsername). If your files appear there before the deadline, they they are submitted.

## Grading criteria

Your program **MUST** compile and run on `flip.engr.oregonstate.edu`, so make sure you have tested your work there before you make your final submission, since a program that compiles and runs in one environment may not compile and run in another.  **Assignments that do not compile on `flip` will receive a grade of 0.**  If you do not have an ENGR account, you can create one at https://teach.engr.oregonstate.edu/.

This assignment is worth 100 points total:
  * 90 points: Your program meets the specifications above
    * 5 points: Implements classes for `Zoo`, `Animal`, `Monkey`, `Sea Otter`, and `Sloth`
    * 10 points: `Monkey`, `Sea Otter`, and `Sloth` classes are derived from the `Animal` class
    * 10 points: Collections of each species are held in dynamically-allocated arrays in the `Zoo` class
    * 10 points: Player can buy new animals, and the cost is correctly deducted from their bank account
    * 10 points: The game correctly calculates the daily cost of food and deducts it from the player's bank account
    * 5 points: the game correctly generates special events (sickness, birth, boom in attendance)
    * 10 points: Sick animals either result in the correct amount being deducted from the cost of the player's bank account, or they die if the player doesn't have enough money
    * 10 points: The birth of babies is correctly implemented, with only adult animals giving birth and babies starting with an age of 0 days.
    * 10 points: A boom in Zoo attendance correctly results in each monkey generating bonus revenue
    * 10 points: Daily revenue is correctly added to the player's bank account based on the number and species of animals in the zoo

  * 10 points: Your code is appropriately commented and uses consistent and appropriate style

In addition, you can earn up to 10 points of extra credit, as described above.

Your work on this assignment will be graded by demoing it with a TA.  During your demo, you will compile and run your program for the TA and walk them through your source code, describing to them how it works.  It'll be your responsibility to sign up for a 10-minute demo slot with a TA by the demo due date above.  **You will receive a zero on the assignment if you don't demo it with a TA by the demo due date.**
