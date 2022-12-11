# std_bot

## What does the bot do?
std_bot searches in r/cpp_questions for STL related functions or types (everything "std::\*", calling this "*token*" from now on) and tries to link them with the reference page from [cppreference.com](https://en.cppreference.com/w/)

If this bot made a reply to your comment with links, the reply is not supposed to help _you_. It's supposed to help the OP (and other readers of your comment) because you introduced potentially new tokens OP doesn't know about. So the links provided by this bot are for OP and other readers to have a quick look at what you're talking about. I'm fully aware that everyone who uses a token knows about it and doesn't need a link.

Be aware that the bot is still in development. Every behaviour is subject to change

If the feedback is mostly negative and I can't get it to work without annoying the users in the sub, I will deactivate it

## TL:DR commands

| Command        | Result                                      |
| -------------- | ------------------------------------------- |
| !std           | Enforce link creation                       |
| !std ignore_me | The bot will ignore you ("!std" will works) |
| !std follow_me | The bot will follow you again               |

## What comments does it respond to and how?

In short terms:

- The bot scans your comment and every other comment in the thread, also OP text and title
- If you used some tokens outside of quotes or code blocks, the bot will link them and put them in a list
  - From this list delete every token whose link has already been used somewhere in the same thread
  - Also delete all tokens from this list OP used in their text
- If there is at least one token left in this list and your comment it is on top level, the bot will post the full list as a reply

To make this a bit more efficient I use caches for indexed comments and links I already searched online for

## How does the bot get it's results?

The bot uses https://en.cppreference.com/w/cpp/symbol_index to look for the symbols. Nested symbols like std::chrono::Monday result in multiple requests. For example:

- -> https://en.cppreference.com/w/cpp/symbol_index

- -> https://en.cppreference.com/w/cpp/symbol_index/chrono

- -> https://en.cppreference.com/w/cpp/chrono/weekday

## How can I give feedback or suggestions?

I'm very grateful for every form of feedback, positive or negative.

The best way is right here in this repository.

## I don't want the bot to respond to my comments

Just reply with "**!std ignore_me**" in any comment and it will ignore your comments from now on

Type "**!std follow_me**" to let it follow you again

Even if ignored you can still use "**!std**" to manually invoke the bot

## I'm still annoyed by the bot

I'm sorry to hear that. Best you can do is to give me some feedback and ignore the bot via Reddit

## ToDo

- [ ] ~~Adding other popular frameworks (like boost::)~~
  - [ ] Probably not possible due to missing search function and symbol index
- [x] Recognizing used links even if they're not bound to the token
- [x] Tokens used by OP should be added to index as OP probably knows about them
  - [ ] This should also work if OP uses the 'using namespace std;' expression (as "std::" normally indicates STL tokens)
