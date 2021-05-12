#pragma once

namespace donut {
    class Application {
    public:
        static std::shared_ptr<Application> Create(int windowWidth, int windowHeight, std::string const& name = "Donut App");
        static std::shared_ptr<Application> GetInstance() { return s_instance; };
        
        virtual ~Application();
        
        char const* GetName() { return m_name.c_str(); }
        std::shared_ptr<IWindow> GetWindow() { return m_window; }

        void Run();

    private:
        static std::shared_ptr<Application> s_instance;

        Application(int windowWidth, int windowHeight, std::string const& name);

        std::string m_name;
        std::shared_ptr<IWindow> m_window;
    };
}