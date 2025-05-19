/**
 * @file Score.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once

class Score {
public:
    Score();
    ~Score();

    void increase(int points);
    int getScore() const;
    void display();

private:    int currentScore;
};
