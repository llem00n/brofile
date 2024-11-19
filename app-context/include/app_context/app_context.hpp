#ifndef BROFILE_APP_CONTEXT_HPP
#define BROFILE_APP_CONTEXT_HPP

#include <filesystem>
#include <memory>

namespace bf {
  namespace ctx { // namespace

    struct context {
      bool new_window;
      bool incognito;
      int selected_browser;
      int selected_profile;
      int selected_firefox_container;
    };

    class app_context : public context {
      static std::unique_ptr<app_context> instance;

     public:
      app_context(const app_context&) = delete;
      app_context& operator=(const app_context&) = delete;
      app_context(app_context&&) = delete;
      app_context& operator=(app_context&&) = delete;

      static app_context& get_instance();
      void init();
      void save();

     private:
      app_context();
      std::filesystem::path get_config_dir();

      void remove_cache_v1();
    };
  }  // namespace ctx
}  // namespace bf

#endif  // BROFILE_APP_CONTEXT_HPP
