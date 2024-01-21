#pragma once


class PlayerScore
{
public:
	static inline void ResetScore() { playerScore = 0; }

	static inline int GetScore() { return playerScore;  }

	static inline void AddScore(int par, int strokesUsed) { playerScore += (par - strokesUsed); }

private:
	static int playerScore;
};
