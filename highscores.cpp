
#include "highscores.h"

highscores* highscores::mInstance = NULL;

highscores* highscores::Instance()
{
	if (!mInstance) 
		mInstance = new highscores;

    return mInstance;
}


highscores::highscores()
{
	Message = nullptr;
	surfaceMessage = nullptr;
	readFile();
	sortScores();
}

int oldsaveCurScore = 0;

void highscores::render()
{

	if (screenManager::Instance()->curScreen == SCREENS::SCREEN_MENU || 
		screenManager::Instance()->curScreen == SCREENS::SCREEN_LEVELCHANGE) return;

	if (!surfaceMessage || curScore != oldsaveCurScore)
	{
		if (surfaceMessage)
			SDL_FreeSurface(surfaceMessage);

		if (Message)
			SDL_DestroyTexture(Message);

		curScoreString = "Score: " + to_string(curScore);
		oldsaveCurScore = curScore;

		surfaceMessage = TTF_RenderText_Solid(gameBase::Instance()->Sans, curScoreString.c_str(), { 255,255,255 });

		Message = SDL_CreateTextureFromSurface(gameBase::Instance()->gRenderer, surfaceMessage);

		Message_rect.x = 5;
		Message_rect.y = 5;
		Message_rect.w = 140;
		Message_rect.h = 60;
	}

    SDL_RenderCopy(gameBase::Instance()->gRenderer, Message, NULL, &Message_rect);
}

fstream myfile;
string line;

void highscores::readFile()
{
	int i = 0;

	myfile.open("highScores/scores.txt");

	if (!myfile.good())
	{
		cout << "No scores found, making file " << endl;
		myfile.open("highScores/scores.txt", fstream::in | fstream::out | fstream::trunc);;

	}

	while (getline(myfile, line))
	{
		regex a("(^.*)\-(.*)");
		smatch matches;

		if (regex_match(line, matches, a)) {

			Score newScore;
			newScore.name = matches[1];
			newScore.score = stoi(matches[2]);

			scores[i] = newScore;

			i++;
		}

	}

	myfile.close();


}

void highscores::writeFile()
{

	myfile.open("highScores/scores.txt");

	for (int i = 0; i < MAX_SCORES; ++i) {

		if (scores[i].score == 0)
			continue;

		string str = scores[i].name + "-" + to_string(scores[i].score) + "\n";
		myfile << str;
	}

	myfile.close();

}

void highscores::addScore(string name, int points)
{
	if (scores[MAX_SCORES - 1].score < points)
	{
		scores[MAX_SCORES - 1].score = points;
		scores[MAX_SCORES - 1].name = name;
	}

	sortScores();
	writeFile();
}

int highscores::getHighestScoreOfInt(int i)
{
	sortScores();

	for (int i = 0; i < MAX_SCORES; ++i) {

		if (scores[i].score == 0 || scores[i].name != to_string(i))
			continue;

		return scores[i].score;
	}

	return 0;
}

void highscores::displayScores()
{

	for (int i = 0; i < MAX_SCORES; ++i) {

		if (scores[i].score == 0)
			continue;

		cout << "" << scores[i].name << ": " << scores[i].score << "\n";
	}

	cout << endl;

}

bool compareScore(highscores::Score a, highscores::Score b)
{
	return (a.score > b.score);
}

void highscores::sortScores()
{
	sort(scores, (scores + MAX_SCORES), compareScore);
}