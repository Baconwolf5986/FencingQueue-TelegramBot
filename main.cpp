#include "StripQueue.h"
#include <iostream>
#include <tgbot/tgbot.h>

int main()
{
	// Initialize TGBot
	TgBot::Bot bot("7267866104:AAEL8Mj-CQahRvHCamI7q8fXYdBIvYA-1fI");

	// Initialize an inline keyboard
	TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
	std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
	std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
	std::vector<TgBot::InlineKeyboardButton::Ptr> row2;

	// Create a Join Queue button for the keyboard
	TgBot::InlineKeyboardButton::Ptr SingleQueueButton(new TgBot::InlineKeyboardButton);
	SingleQueueButton->text = "Join Queue";
	SingleQueueButton->callbackData = "joinSingle_callback";
	row0.push_back(SingleQueueButton);

	// Create a Join Queue Pair for the keyboard
	TgBot::InlineKeyboardButton::Ptr PairQueueButton(new TgBot::InlineKeyboardButton);
	PairQueueButton->text = "Join as Pair";
	PairQueueButton->callbackData = "joinPair_callback";
	row0.push_back(PairQueueButton);

	// Create a Next Bout button
	TgBot::InlineKeyboardButton::Ptr NextBoutButton(new TgBot::InlineKeyboardButton);
	NextBoutButton->text = "Next Bout";
	NextBoutButton->callbackData = "nextBout_callback";
	row1.push_back(NextBoutButton);

	// Create a Show Queue button
	TgBot::InlineKeyboardButton::Ptr ShowQueueButton(new TgBot::InlineKeyboardButton);
	ShowQueueButton->text = "Show Queue";
	ShowQueueButton->callbackData = "showQueue_callback";
	row1.push_back(ShowQueueButton);
	
	TgBot::InlineKeyboardButton::Ptr AddNameButton(new TgBot::InlineKeyboardButton);
	AddNameButton->text = "Add Name";
	AddNameButton->callbackData = "addName_callback";
	row2.push_back(AddNameButton);

	keyboard->inlineKeyboard.push_back(row0);
	keyboard->inlineKeyboard.push_back(row1);
	keyboard->inlineKeyboard.push_back(row2);

	// Initialize Queue
	StripQueue stripQueue;

	std::string pairingFencer;

	// Handler for /start commands
	// Change to help or something
	bot.getEvents().onCommand("start", [&bot, &keyboard, &stripQueue](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Fencing Queue Created! \n"
			"Type /help for information on commands \n"
			"Type /start to reset the queue\n"
			"Press [Add Name] to add your name to the list (used for joining as a pair)"
			,
			false, 0, keyboard
		);
		StripQueue newStripQueue;
		stripQueue = newStripQueue;

		});

	bot.getEvents().onCommand("help", [&bot, &keyboard](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Commands: \n"
			"/start - Reset the queue\n"
			"/help - Display this message\n"
			"[Join Queue] - Join the queue as a single fencer\n"
			"[Join as Pair] - Join the queue as a pair with another fencer (They must have already joined the queue or added their name with the [Add Name] button\n"
			"[Next Bout] - Display the next bout\n"
			"[Show Queue] - Display the current queue\n"
			"[Add Name] - Add your name to the list of fencers\n"
			,
			false, 0, keyboard
		);
		});

	bot.getEvents().onCallbackQuery([&bot, &stripQueue, &keyboard, &pairingFencer](TgBot::CallbackQuery::Ptr query) {
		// JOIN SINGLE BUTTON REFERENCE
		if (query->data == "joinSingle_callback") {
			std::string firstName = query->from->firstName;

			std::string outputMessage = stripQueue.AddFencerSingle(firstName)
				+ "\nCurrent Queue: \n";
			outputMessage += stripQueue.PrintQueue();
			bot.getApi().sendMessage(query->message->chat->id, outputMessage, false, 0, keyboard);
		}
		// JOIN PAIR BUTTON REFERENCE
		else if (query->data == "joinPair_callback")
		{	
			// New Keyboard, every button will be the name of another fencer in the list
			TgBot::InlineKeyboardMarkup::Ptr nameKeyboard(new TgBot::InlineKeyboardMarkup);
			for (auto& fencer : stripQueue.m_QueuedFencers)
			{
				std::vector<TgBot::InlineKeyboardButton::Ptr> name;
				TgBot::InlineKeyboardButton::Ptr nameButton(new TgBot::InlineKeyboardButton);
				nameButton->text = fencer.first;
				nameButton->callbackData = fencer.first;
				name.push_back(nameButton);
				nameKeyboard->inlineKeyboard.push_back(name);
			}

			std::string firstName = query->from->firstName;
			pairingFencer = firstName;
			bot.getApi().sendMessage(query->message->chat->id, "Please select a user to queue with"
			,false, 0, nameKeyboard);
		}
		// NEXT BOUT BUTTON REFERENCE
		else if (query->data == "nextBout_callback")
		{
			std::string outputMessage = "Next Bout: ";
			outputMessage += stripQueue.nextBout();
			outputMessage += "\nCurrent Queue: \n";
			outputMessage += stripQueue.PrintQueue();
			bot.getApi().sendMessage(query->message->chat->id, outputMessage, false, 0, keyboard);
		}
		// SHOW QUEUE BUTTON REFERENCE
		else if (query->data == "showQueue_callback")
		{
			std::string outputMessage = "Current Queue:\n";
			outputMessage += stripQueue.PrintQueue();
			bot.getApi().sendMessage(query->message->chat->id, outputMessage, false, 0, keyboard);
		}
		// ADD NAME BUTTON REFERENCE
		else if (query->data == "addName_callback")
		{
			std::string outputMessage;
			std::string fencerName = query->from->firstName;

			outputMessage = stripQueue.addToMap(fencerName);
			bot.getApi().sendMessage(query->message->chat->id, outputMessage, false, 0, keyboard);
		}
		// BUTTON (FOR EACH NAME) FOR PAIRING
		else if (query->data == stripQueue.findInMap(query->data))
		{
			std::string outputMessage = stripQueue.AddFencerPair(pairingFencer, query->data);
			outputMessage += "\nCurrent Queue: \n";
			outputMessage += stripQueue.PrintQueue();

			bot.getApi().sendMessage(query->message->chat->id, outputMessage, false, 0, keyboard);
		}
		});

	// Handler for /joinQueue
	// Adds a single fencer to the queue
	// Single argument should be fencer name
	bot.getEvents().onCommand("Add Name", [&bot, &stripQueue](TgBot::Message::Ptr message)
		{
			
		});

	bot.getEvents().onCommand("dev", [&bot, &stripQueue](TgBot::Message::Ptr message) {
		stripQueue.PopulateQueue();
	});

	try {
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		TgBot::TgLongPoll longPoll(bot);
		while (true) {
			printf("Long poll started\n");
			longPoll.start();
		}
	}
	catch (TgBot::TgException& e) {
		printf("error: %s\n", e.what());
	}

	return 0;
}