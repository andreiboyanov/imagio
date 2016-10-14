#include "imagio_sdl.h"
#include "imgui.h"
#include <stdlib.h>
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif


SDL_Surface *window;


bool ImGuiSdl::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	atexit(SDL_Quit);

	window = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	if (window == NULL)
	{
		fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
		exit(1);
	}

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	io.RenderDrawListsFn = NULL; // ImGuiSdl::Render;
	// io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
	// io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
#ifdef _WIN32
	// io.ImeWindowHandle = glfwGetWin32Window(g_Window);
#endif

	return true;
}


int ImGuiSdl::PrepareNewFrame() {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;

		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			io.MousePos.x = event.motion.x;
			io.MousePos.y = event.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			io.MouseDown[0] = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			io.MouseDown[0] = 0;
			break;
		case SDL_KEYDOWN:
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return 1;
			break;
		case SDL_QUIT:
			return 1;
		}
	}
	return 0;
}

void ImGuiSdl::Render() {
	if (SDL_MUSTLOCK(window))
		if (SDL_LockSurface(window) < 0)
			return;

	int tick = SDL_GetTicks();

	if (SDL_MUSTLOCK(window))
		SDL_UnlockSurface(window);

	SDL_UpdateRect(window, 0, 0, 640, 480);
}