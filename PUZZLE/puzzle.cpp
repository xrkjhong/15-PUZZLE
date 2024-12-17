#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace sf;

int main()
{

    RenderWindow app(VideoMode(400, 400), "15-Puzzle!");
    app.setFramerateLimit(60); 

    Font font;
    if (!font.loadFromFile("ARCADE_I.TTF"))
    {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    int w = 100; 
    int grid[6][6] = { 0 }; 
    Text text[20]; 

    
    int n = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            n++; 
            grid[i + 1][j + 1] = n; 
            text[n].setFont(font);
            text[n].setString(std::to_string(n)); 
            text[n].setCharacterSize(48);
            text[n].setFillColor(Color::White); 
            FloatRect textBounds = text[n].getLocalBounds();
            text[n].setOrigin(textBounds.width / 2, textBounds.height / 2); 
        }

    grid[4][4] = 16; 

   
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < 1000; i++) 
    {
        int x = 4; 
        int y = 4;

        int dx = 0, dy = 0; 
        int dir = rand() % 4; 
        if (dir == 0) dx = -1;
        if (dir == 1) dx = 1;
        if (dir == 2) dy = -1;
        if (dir == 3) dy = 1;

        if (grid[x + dx][y + dy] != 0 && x + dx >= 1 && x + dx <= 4 && y + dy >= 1 && y + dy <= 4)
        {
            std::swap(grid[x][y], grid[x + dx][y + dy]); 
        }
    }

    bool isWin = false; 

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e)) 
        {
            if (e.type == Event::Closed) 
                app.close();

            if (e.type == Event::KeyPressed && !isWin) 
            {
                int x = 0, y = 0;

                for (int i = 1; i <= 4; i++)
                    for (int j = 1; j <= 4; j++)
                        if (grid[i][j] == 16)
                        {
                            x = i;
                            y = j;
                        }

                int dx = 0, dy = 0; 
                if (e.key.code == Keyboard::Left)  dx = 0, dy = 1;  
                if (e.key.code == Keyboard::Right) dx = 0, dy = -1;
                if (e.key.code == Keyboard::Up)    dx = 1, dy = 0;  
                if (e.key.code == Keyboard::Down)  dx = -1, dy = 0; 

                if (grid[x + dx][y + dy] != 0 && x + dx >= 1 && x + dx <= 4 && y + dy >= 1 && y + dy <= 4)
                {
                    std::swap(grid[x][y], grid[x + dx][y + dy]); 
                }
            }
        }

        isWin = true;
        int n = 0;
        for (int i = 1; i <= 4; i++)
            for (int j = 1; j <= 4; j++)
            {
                n++;
                if (n == 16) break; 
                if (grid[i][j] != n) isWin = false; 
            }

        app.clear(Color::White); 
        
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                int n = grid[i + 1][j + 1];
                if (n == 16) continue; 

                RectangleShape block(Vector2f(w - 2, w - 2));
                block.setFillColor(Color::Black);
                block.setPosition(j * w + 1, i * w + 1);
                app.draw(block);

                text[n].setPosition(j * w + w / 2, i * w + w / 2);
                app.draw(text[n]);
            }

        if (isWin) 
        {
            Text winText;
            winText.setFont(font);
            winText.setString("YOU WIN!");
            winText.setCharacterSize(48);
            winText.setFillColor(Color::Red);
            FloatRect textBounds = winText.getLocalBounds();
            winText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            winText.setPosition(app.getSize().x / 2, app.getSize().y / 2);
            app.draw(winText);
        }

        app.display(); 
    }

    return 0; 
}
