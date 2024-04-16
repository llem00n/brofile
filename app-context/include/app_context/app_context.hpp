#ifndef BROFILE_APP_CONTEXT_HPP
#define BROFILE_APP_CONTEXT_HPP

#include <memory>
#include <filesystem>

namespace bf {
  namespace ctx {
    namespace _ctx {
      struct ctx {
        bool new_window;
        bool incognito;
        int selected_browser;
        int selected_profile;
      };
    }

    class app_context {
      static std::unique_ptr<app_context> instance;
      _ctx::ctx context;

    public:
      app_context(const app_context&) = delete;
      app_context& operator=(const app_context&) = delete;
      app_context(app_context&&) = delete;
      app_context& operator=(app_context&&) = delete;

      static app_context& get_instance();
      void init();
      void save();

      _ctx::ctx &get_context();
    private:
      app_context();
      std::filesystem::path get_config_dir();
    };
  }
}

#endif //BROFILE_APP_CONTEXT_HPP
