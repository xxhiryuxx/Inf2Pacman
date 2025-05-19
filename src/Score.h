#pragma once

class Score {
public:
    Score();
    ~Score();

    void increase(int points);
    int getScore() const;
    void display();

private:
    int currentScore;
};

#endif // SCORE_H
