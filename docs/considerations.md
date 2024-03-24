> The vkGetInstanceProcAddr function will return nullptr if the function couldn't be loaded.

Hmm.  So I should probably check for that in my ugly preprocessor work.

Not checking any return codes from GLFW yet.
