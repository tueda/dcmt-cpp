set(DOXYGEN_EXTRACT_ALL "YES")
set(DOXYGEN_GENERATE_XML "YES") # for Sphinx + Breathe
set(DOXYGEN_JAVADOC_AUTOBRIEF "YES")
set(DOXYGEN_PROJECT_NAME "dcmt-cpp")
set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}")
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")

doxygen_add_docs(doc "${PROJECT_SOURCE_DIR}/README.md"
                 "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src")
