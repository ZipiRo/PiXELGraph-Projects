#include <Core/PiXELGraph.h>

#include "Widget.h"
#include "Slider.h"
#include "Button.h"

struct Ball
{
    Shapes::Elipse elipse;

    Vector2 position;
    Vector2 velocity;
    Vector2 force;

    float mass;
    float radius;

    void Step()
    {
        Vector2 acc = force / mass;

        velocity += acc * Time::deltaTime;
        position += velocity * Time::deltaTime;

        velocity *= 0.99f;

        force = Vector2::ZERO;
    }

    void Draw()
    {
        elipse.transform.SetPosition(position);
        DrawShape(elipse);
    }
};

class GUIScene : public PiXELGraph
{
private:

    Vector2 gravity;

    Slider rs;
    Slider gs;
    Slider bs;

    Slider grs;

    Slider angle;
    Slider zoom;

    Slider sidess;
    Button b;

    Text rvalue;
    Text gvalue;
    Text bvalue;
    Text grvvalue;
    Text angvalue;
    Text zoomvalue;

    Ball ball;

    View canvas;
    View camera;

    Shapes::Elipse elipse;

    float sidesValue;
    float zoomValue;

    Clip hit_sfx;

    void Start() override
    {
        hit_sfx = Clip("hit.mp3");
        camera = canvas = Screen::GetView();

        gravity = Vector2(0, 9.81f);

        ball.elipse = Shapes::Elipse(5, 5, 10);
        ball.elipse.SetFillColor(Color::Blue);

        ball.position = Vector2(100, 250);

        ball.mass = 1.0f;
        ball.radius = 5.0f;

        rs = Slider(10, 100, 50, 10);
        rs.fillColor = Color::Red;
        rs.outlineColor = Color::Transparent;
        rs.handleColor = Color(200,200,200);

        gs = Slider(10, 120, 50, 10);
        gs.fillColor = Color::Green;
        gs.outlineColor = Color::Transparent;
        gs.handleColor = Color(200,200,200);
        
        bs = Slider(10, 140, 50, 10);
        bs.fillColor = Color::Blue;
        bs.outlineColor = Color::Transparent;
        bs.handleColor = Color(200,200,200);
        
        sidess = Slider(10, 180, 50, 10);
        sidess.fillColor = Color(150,150,150);
        sidess.outlineColor = Color::Transparent;
        sidess.handleColor = Color(200,200,200);
        
        grs = Slider(10, 200, 50, 10);
        grs.fillColor = Color(150,150,150);
        grs.outlineColor = Color::Transparent;
        grs.handleColor = Color(200,200,200);
        
        angle = Slider(10, 220, 50, 10);
        angle.fillColor = Color(150,150,150);
        angle.outlineColor = Color::Transparent;
        angle.handleColor = Color(200,200,200);

        zoom = Slider(120, 10, 50, 10);
        zoom.fillColor = Color(150,150,150);
        zoom.outlineColor = Color::Transparent;
        zoom.handleColor = Color(200,200,200);
        zoom.SetValue(.5f);

        b = Button(10, 240, 70, 20, "Apply Force");
        b.fillColor = Color(200, 200, 200);
        b.outlineColor = Color::Black;
        b.SetOnClick([this]()
        {
            Debug::Log("PRESSED");
            ball.velocity += newForce;
        });

        rvalue.SetColor(Color::White);
        rvalue.transform.SetPosition(10, 10);
        rvalue.transform.SetScale(5, 5);
        
        gvalue.SetColor(Color::White);
        gvalue.transform.SetPosition(10, 20);
        gvalue.transform.SetScale(5, 5);
        
        bvalue.SetColor(Color::White);
        bvalue.transform.SetPosition(10, 30);
        bvalue.transform.SetScale(5, 5);

        grvvalue.SetColor(Color::White);
        grvvalue.transform.SetPosition(10, 40);
        grvvalue.transform.SetScale(5, 5);
        
        angvalue.SetColor(Color::White);
        angvalue.transform.SetPosition(10, 50);
        angvalue.transform.SetScale(5, 5);
        
        zoomvalue.SetColor(Color::White);
        zoomvalue.transform.SetPosition(10, 60);
        zoomvalue.transform.SetScale(5, 5);

        elipse = Shapes::Elipse(30, 30);
        elipse.transform.SetPosition(40, 100);
        elipse.SetFillColor(Color::Red);   
    }

    Vector2 newForce;
    Vector2 forceDirection;
    Vector2 viewBounds;

    void Update() override
    {
        Vector2 mousePosition = Input::MousePosition / Window::WindowFontSize();

        viewBounds = Vector2(Screen::Width(), Screen::Height()) * 2 * zoomValue;
        viewBounds = camera.ScreenToWorld(viewBounds);

        rs.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));
        rvalue.SetString("R:" + std::to_string(int(255 * rs.GetValue())));

        gs.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));
        gvalue.SetString("G:" + std::to_string(int(255 * gs.GetValue())));

        bs.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));
        bvalue.SetString("B:" + std::to_string(int(255 * bs.GetValue())));

        b.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));
        
        grs.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));
        grvvalue.SetString("Magnitude:" + std::to_string(1000 * grs.GetValue()));
        
        angle.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));
        angvalue.SetString("Angle:" + std::to_string(4 * PI * angle.GetValue()));

        angle.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));
        angvalue.SetString("Angle:" + std::to_string(4 * PI * angle.GetValue()));

        zoom.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));
        zoomvalue.SetString("Zoom:" + std::to_string(2 * zoom.GetValue()));

        if(zoomValue != zoom.GetValue())
        {
            camera.SetZoom(2 * zoom.GetValue());
        }

        float angleS = 4 * PI * angle.GetValue();
        forceDirection = Vector2(cos(angleS), sin(angleS));
        newForce = forceDirection * 1000 * grs.GetValue();

        ball.force += gravity * ball.mass;

        ball.Step();

        if(ball.position.y + ball.radius >= 300)
        {
            ball.position.y = 300 - ball.radius;
            ball.velocity = Vector2(ball.velocity.x, -ball.velocity.y);
            ball.velocity *= 0.97f;
            if(Vector2::Length(ball.velocity) > 0.25f)
                AudioSource::PlaySound(hit_sfx);
        }

        if(ball.position.x + ball.radius >= 400)
        {
            ball.position.x = 400 - ball.radius;
            ball.velocity = Vector2(-ball.velocity.x, ball.velocity.y);
            ball.velocity *= 0.97f;
            if(Vector2::Length(ball.velocity) > 0.25f)
                AudioSource::PlaySound(hit_sfx);
        }

        if(ball.position.x - ball.radius <= 0)
        {
            ball.position.x = ball.radius;
            ball.velocity = Vector2(-ball.velocity.x, ball.velocity.y);
            ball.velocity *= 0.97f;
            if(Vector2::Length(ball.velocity) > 0.25f)
                AudioSource::PlaySound(hit_sfx);
        }

        if(ball.position.y - ball.radius <= 0)
        {
            ball.position.y = ball.radius;
            ball.velocity = Vector2(ball.velocity.x, -ball.velocity.y);
            ball.velocity *= 0.97f;
            if(Vector2::Length(ball.velocity) > 0.25f)
                AudioSource::PlaySound(hit_sfx);
        }

        sidess.Update(mousePosition, Input::IsMouseButton(MouseButton::Left));

        Screen::BacgroundColor = Color(int(255 * rs.GetValue()), int(255 * gs.GetValue()), int(255 * bs.GetValue()));

        if(sidesValue != sidess.GetValue())
        {
            elipse = Shapes::Elipse(30, 30, 15 * sidess.GetValue());
            elipse.transform.SetPosition(40, 50);
            elipse.SetFillColor(Color::Red);
        }

        sidesValue = sidess.GetValue();
        zoomValue = zoom.GetValue();
    }

    void Draw() override
    {
        // Screen::SetView(camera);
        ball.Draw();

        DrawShape(elipse);

        DrawLines({
            {{ball.position}, Color::Green},
            {{ball.position + forceDirection * grs.GetValue() * 1000}, Color::Green},
        });

        DrawLines({
            {{ball.position}, Color::Red},
            {{ball.position + ball.velocity}, Color::Red},
        });

        Screen::SetView(canvas);

        rs.Draw();
        gs.Draw();
        bs.Draw();
        b.Draw();
        sidess.Draw();
        zoom.Draw();

        grs.Draw();
        angle.Draw();

        DrawTEXT(rvalue);
        DrawTEXT(gvalue);
        DrawTEXT(bvalue);
        DrawTEXT(grvvalue);
        DrawTEXT(angvalue);
        DrawTEXT(zoomvalue);

        Screen::SetView(camera);
    }

public:
    GUIScene()
    {
        Init(800, 600, 1, L"GUIDemo");
    }
};