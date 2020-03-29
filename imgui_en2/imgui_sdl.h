// MIT License

// Copyright (c) 2018 

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

struct ImDrawData;
struct SDL_Renderer;

namespace ImGuiSDL
{
	// Call this to initialize the SDL renderer device that is internally used by the renderer.
	void Initialize(SDL_Renderer* renderer, int windowWidth, int windowHeight);
	// Call this before destroying your SDL renderer or ImGui to ensure that proper cleanup is done. This doesn't do anything critically important though,
	// so if you're fine with small memory leaks at the end of your application, you can even omit this.
	void Deinitialize();

	// Call this every frame after ImGui::Render with ImGui::GetDrawData(). This will use the SDL_Renderer provided to the interfrace with Initialize
	// to draw the contents of the draw data to the screen.
	void Render(ImDrawData* drawData);
}
