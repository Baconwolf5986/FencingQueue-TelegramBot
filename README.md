# FencingQueue-TelegramBot

A Telegram Bot used to automate the queue process for those waiting for a fencing strip. 
Written in C++, using the tgbot-cpp library. 

## Goals

- Create a simple queue system that the fencing club can use (Everyone uses telegram)
- Prevent fencers from being at the front of the queue while they're still fencing
- Easier sign up to the queue alone (Does not need to sign up with another person)
- Prevent unintentionally fencing the same person twice (But still possible)
- Allow a pair of two fencers to sign up together

## Features

### Join Queue
  - Adds the user's name (the one who pressed the button) to the queue as a single fencer
  - If there is another single fencer in the queue they are matched up together
  - Single fencers will maintain their position in the queue even if they are not paired with another fencer
  - Pressing join queue when the other single fencer is someone you have already fenced will not allow you to join the queue
### Join as Pair
  - Sends a new message asking who the user wants to fence
  - Lists the first names of all other users who have already queued up (or have added their name using the 'Add Name' button)
  - Pairs the user with the user whose name is pressed
### Next Bout
  - Outputs and removes the names of the fencing match at the top of the queue
  - Outputs resulting queue after the removal
  - Skips the top result if it is a solo fencer (but preserves their place at the top)
### Show Queue
  - Outputs every member of the queue
  - Single fencers are said to be waiting for an opponent
### Add Name
  - Adds the user's first name to the list of fencers without needing to queue up first
  - Necessary for 'Join as Pair' button
 
### Features still needed

- [ ] Leave Queue (Implementation and Button)
- [ ] Button Permissions (For 'Next Bout' button)
- [ ] 'Join as Pair' request (Ask for permission to join as pair)
- [ ] Yes or No buttons (For when 'Join Queue' detects a repeat bout)


## System and Dependencies

Dependencies:
- CMake
- Boost
- OpenSSL
- ZLib
- tgbot-cpp

Can be used on any system with CMake, but dependencies must be installed according using the steps described on the README of tg-botcpp. https://github.com/reo7sp/tgbot-cpp.
