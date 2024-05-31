/*
Ходить: WASD
Вертеть: L и K
Стрелять: ПРОБЕЛ
*/


#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <ctime>

int randomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(generator);
}

class Bob : public sf::Sprite
{
public:
    Bob(const sf::Texture& texture) : sf::Sprite(texture)
    {
        setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
    }

    void move(float dx, float dy)
    {
        sf::Sprite::move(dx, dy);
    }

    void rotate(float angle)
    {
        setRotation(getRotation() + angle);
    }

    sf::Vector2f getPosition() const
    {
        return sf::Sprite::getPosition();
    }
};

class Pat : public sf::Sprite
{
public:
    Pat(const sf::Texture& texture) : sf::Sprite(texture)
    {
    }

    void update(sf::RenderWindow& window)
    {
        move(0.5f, 0.f);
        if (getPosition().x > window.getSize().x)
        {
            setPosition(-getLocalBounds().width, randomInt(0, window.getSize().y - getLocalBounds().height));
        }
    }
};

class Burger : public sf::Sprite
{
public:
    Burger(const sf::Texture& texture) : sf::Sprite(texture)
    {
        setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
    }

    void move(float dx, float dy)
    {
        sf::Sprite::move(dx, dy);
    }

    void rotate(float angle)
    {
        setRotation(angle);
    }

    sf::FloatRect getGlobalBounds() const
    {
        return sf::Sprite::getGlobalBounds();
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(2000, 2000), "Burger Shooter");
    window.setFramerateLimit(60);

    sf::Texture bobTexture;
    if (!bobTexture.loadFromFile("assets/bob.png"))
    {
        std::cerr << "bob.png not found" << std::endl;
        return 1;
    }
    sf::Texture burgerTexture;
    if (!burgerTexture.loadFromFile("assets/burger.png"))
    {
        std::cerr << "burger.png not found" << std::endl;
        return 1;
    }
    sf::Texture patTexture;
    if (!patTexture.loadFromFile("assets/pat.png"))
    {
        std::cerr << "pat.png not found" << std::endl;
        return 1;
    }
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/bik.jpg"))
    {
        std::cerr << "bik.jpg not found" << std::endl;
        return 1;
    }

    Bob bob(bobTexture);
    bob.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    Pat pat(patTexture);

    std::vector<Burger> burgers;

    sf::Sprite background(backgroundTexture);
    background.setScale(float(window.getSize().x) / backgroundTexture.getSize().x,
        float(window.getSize().y) / backgroundTexture.getSize().y);

    sf::Clock patTimer;
    float patSpawnInterval = 3.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            bob.move(0, -5.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            bob.move(0, 5.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            bob.move(-5.f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            bob.move(5.f, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        {
            bob.rotate(-5.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            bob.rotate(5.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            Burger newBurger(burgerTexture);
            newBurger.setPosition(bob.getPosition());
            newBurger.rotate(bob.getRotation());
            burgers.push_back(newBurger);
        }

        for (auto& burger : burgers)
        {
            float angle = burger.getRotation() * 3.14159f / 180.f;
            float dx = std::cos(angle) * 10.f;
            float dy = std::sin(angle) * 10.f;
            burger.move(dx, dy);
        }

        if (patTimer.getElapsedTime().asSeconds() > patSpawnInterval)
        {
            pat.setPosition(randomInt(0, window.getSize().x - pat.getLocalBounds().width),
                randomInt(0, window.getSize().y - pat.getLocalBounds().height));
            patTimer.restart();
        }

        pat.update(window);

        for (int i = 0; i < burgers.size(); i++)
        {
            if (burgers[i].getGlobalBounds().intersects(pat.getGlobalBounds()))
            {
                pat.setPosition(-pat.getLocalBounds().width, -pat.getLocalBounds().height);
                burgers.erase(burgers.begin() + i);
            }
        }

        window.clear();

        window.draw(background);

        window.draw(pat);

        for (auto& burger : burgers)
        {
            window.draw(burger);
        }

        window.draw(bob);

        window.display();
    }

    return 0;
}