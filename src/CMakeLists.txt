add_executable(qmlgreet
    main.cpp
    backend.cpp
    resources.qrc
)

target_link_libraries(qmlgreet
    Qt::Core
    Qt::Gui
    Qt::Quick
)

set_target_properties(qmlgreet PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    CXX_STANDARD 14
)

install(TARGETS qmlgreet DESTINATION bin)
