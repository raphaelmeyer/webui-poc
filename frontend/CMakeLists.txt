add_custom_target(frontend ALL
  COMMAND npm run build
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}
  BYPRODUCTS
    build/index.html
  SOURCES
    package.json
    src/App.js
    src/index.js
    src/index.css
    src/components/machine.js
)
