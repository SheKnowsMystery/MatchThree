module;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

export module components.sdl:bindings;

export
namespace sdl
{
	using Point         = SDL_Point;
	using FPoint        = SDL_FPoint;
	using Rect          = SDL_Rect;
	using FRect         = SDL_FRect;
	using Color         = SDL_Color;
	using Window        = SDL_Window;
	using Renderer      = SDL_Renderer;
	using Surface       = SDL_Surface;
	using Texture       = SDL_Texture;
	using Event         = SDL_Event;
	using EventType     = SDL_EventType;
	using WindowEventID = SDL_WindowEventID;
} // namespace sdl

export
namespace sdl::env
{
	constexpr Color black = {.r = 0,   .g = 0,   .b = 0,   .a = 255};
	constexpr Color white = {.r = 255, .g = 255, .b = 255, .a = 255};
	constexpr Color gray  = {.r = 200, .g = 200, .b = 200, .a = 255};
} // namespace sdl::env

export
namespace sdl::win::pos
{
	/**
	 *  \brief Used to indicate that you don't care what the window position is.
	 */
	namespace undefined
	{
		constexpr Uint32 mask = 0x1FFF0000u;
		constexpr Uint32 display(Uint32 _pos) { return mask | _pos; }
	} // namespace undefined
	constexpr bool is_undefined(Uint32 _pos) { return (_pos & 0xFFFF0000) == undefined::mask; }

	/**
	 *  \brief Used to indicate that the window position should be centered.
	 */
	namespace centered
	{
		constexpr Uint32 mask = 0x2FFF0000u;
		constexpr Uint32 display(Uint32 _pos) { return mask | _pos; }
	} // namespace centered
	constexpr bool is_centered(Uint32 _pos) { return (_pos & 0xFFFF0000) == centered::mask; }
} // namespace sdl::win::pos

export
namespace sdl::init
{
	enum : Uint32
	{
		TIMER          = SDL_INIT_TIMER         ,
		AUDIO          = SDL_INIT_AUDIO         ,
		VIDEO          = SDL_INIT_VIDEO         , // VIDEO implies EVENTS
		JOYSTICK       = SDL_INIT_JOYSTICK      , // JOYSTICK implies EVENTS
		HAPTIC         = SDL_INIT_HAPTIC        ,
		GAMECONTROLLER = SDL_INIT_GAMECONTROLLER, // GAMECONTROLLER implies JOYSTICK
		EVENTS         = SDL_INIT_EVENTS        ,
		SENSOR         = SDL_INIT_SENSOR        ,
		NOPARACHUTE    = SDL_INIT_NOPARACHUTE   , // compatibility; this flag is ignored.
		EVERYTHING     = SDL_INIT_EVERYTHING    ,
	};
} // namespace sdl::init

export
namespace sdl::win::init
{
	enum
	{
		FULLSCREEN         = SDL_WINDOW_FULLSCREEN        , // fullscreen window
		OPENGL             = SDL_WINDOW_OPENGL            , // window usable with OpenGL context
		SHOWN              = SDL_WINDOW_SHOWN             , // window is visible
		HIDDEN             = SDL_WINDOW_HIDDEN            , // window is not visible
		BORDERLESS         = SDL_WINDOW_BORDERLESS        , // no window decoration
		RESIZABLE          = SDL_WINDOW_RESIZABLE         , // window can be resized
		MINIMIZED          = SDL_WINDOW_MINIMIZED         , // window is minimized
		MAXIMIZED          = SDL_WINDOW_MAXIMIZED         , // window is maximized
		MOUSE_GRABBED      = SDL_WINDOW_MOUSE_GRABBED     , // window has grabbed mouse input
		INPUT_FOCUS        = SDL_WINDOW_INPUT_FOCUS       , // window has input focus
		MOUSE_FOCUS        = SDL_WINDOW_MOUSE_FOCUS       , // window has mouse focus
		FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
		FOREIGN            = SDL_WINDOW_FOREIGN           , // window not created by SDL
		ALLOW_HIGHDPI      = SDL_WINDOW_ALLOW_HIGHDPI     , // window should be created in high-DPI mode if supported.
														/* On macOS NSHighResolutionCapable must be set true in the
															application's Info.plist for this to have any effect. */
		MOUSE_CAPTURE      = SDL_WINDOW_MOUSE_CAPTURE     , // window has mouse captured (unrelated to MOUSE_GRABBED)
		ALWAYS_ON_TOP      = SDL_WINDOW_ALWAYS_ON_TOP     , // window should always be above others
		SKIP_TASKBAR       = SDL_WINDOW_SKIP_TASKBAR      , // window should not be added to the taskbar
		UTILITY            = SDL_WINDOW_UTILITY           , // window should be treated as a utility window
		TOOLTIP            = SDL_WINDOW_TOOLTIP           , // window should be treated as a tooltip
		POPUP_MENU         = SDL_WINDOW_POPUP_MENU        , // window should be treated as a popup menu
		KEYBOARD_GRABBED   = SDL_WINDOW_KEYBOARD_GRABBED  , // window has grabbed keyboard input
		VULKAN             = SDL_WINDOW_VULKAN            , // window usable for Vulkan surface
		METAL              = SDL_WINDOW_METAL             , // window usable for Metal view

		INPUT_GRABBED      = SDL_WINDOW_INPUT_GRABBED     , // equivalent to MOUSE_GRABBED for compatibility
	};
} // namespace sdl::win::init

export
namespace sdl::event
{
	enum
	{
		FIRSTEVENT = SDL_FIRSTEVENT, /**< Unused (do not remove) */

		/* Application events */
		QUIT = SDL_QUIT, /**< User-requested quit */

		/* These application events have special meaning on iOS, see README-ios.md for details */
		APP_TERMINATING = SDL_APP_TERMINATING,         /**< The application is being terminated by the OS
		                                  Called on iOS in applicationWillTerminate()
		                                  Called on Android in onDestroy()
		                             */
		APP_LOWMEMORY = SDL_APP_LOWMEMORY,           /**< The application is low on memory, free memory if possible.
		                                  Called on iOS in applicationDidReceiveMemoryWarning()
		                                  Called on Android in onLowMemory()
		                             */
		APP_WILLENTERBACKGROUND = SDL_APP_WILLENTERBACKGROUND, /**< The application is about to enter the background
		                                 Called on iOS in applicationWillResignActive()
		                                 Called on Android in onPause()
		                            */
		APP_DIDENTERBACKGROUND = SDL_APP_DIDENTERBACKGROUND,  /**< The application did enter the background and may not get CPU for some time
		                                  Called on iOS in applicationDidEnterBackground()
		                                  Called on Android in onPause()
		                             */
		APP_WILLENTERFOREGROUND = SDL_APP_WILLENTERFOREGROUND, /**< The application is about to enter the foreground
		                                 Called on iOS in applicationWillEnterForeground()
		                                 Called on Android in onResume()
		                            */
		APP_DIDENTERFOREGROUND = SDL_APP_DIDENTERFOREGROUND,  /**< The application is now interactive
		                                  Called on iOS in applicationDidBecomeActive()
		                                  Called on Android in onResume()
		                             */

		LOCALECHANGED = SDL_LOCALECHANGED, /**< The user's locale preferences have changed. */

		/* Display events */
		DISPLAYEVENT = SDL_DISPLAYEVENT, /**< Display state change */

		/* Window events */
		WINDOWEVENT = SDL_WINDOWEVENT, /**< Window state change */
		SYSWMEVENT = SDL_SYSWMEVENT,          /**< System specific event */

		/* Keyboard events */
		KEYDOWN = SDL_KEYDOWN, /**< Key pressed */
		KEYUP = SDL_KEYUP,           /**< Key released */
		TEXTEDITING = SDL_TEXTEDITING,     /**< Keyboard text editing (composition) */
		TEXTINPUT = SDL_TEXTINPUT,       /**< Keyboard text input */
		KEYMAPCHANGED = SDL_KEYMAPCHANGED,   /**< Keymap changed due to a system event such as an
		                          input language or keyboard layout change.
		                     */
		TEXTEDITING_EXT = SDL_TEXTEDITING_EXT, /**< Extended keyboard text editing (composition) */

		/* Mouse events */
		MOUSEMOTION = SDL_MOUSEMOTION, /**< Mouse moved */
		MOUSEBUTTONDOWN = SDL_MOUSEBUTTONDOWN,     /**< Mouse button pressed */
		MOUSEBUTTONUP = SDL_MOUSEBUTTONUP,       /**< Mouse button released */
		MOUSEWHEEL = SDL_MOUSEWHEEL,          /**< Mouse wheel motion */

		/* Joystick events */
		JOYAXISMOTION = SDL_JOYAXISMOTION, /**< Joystick axis motion */
		JOYBALLMOTION = SDL_JOYBALLMOTION,         /**< Joystick trackball motion */
		JOYHATMOTION = SDL_JOYHATMOTION,          /**< Joystick hat position change */
		JOYBUTTONDOWN = SDL_JOYBUTTONDOWN,         /**< Joystick button pressed */
		JOYBUTTONUP = SDL_JOYBUTTONUP,           /**< Joystick button released */
		JOYDEVICEADDED = SDL_JOYDEVICEADDED,        /**< A new joystick has been inserted into the system */
		JOYDEVICEREMOVED = SDL_JOYDEVICEREMOVED,      /**< An opened joystick has been removed */
		JOYBATTERYUPDATED = SDL_JOYBATTERYUPDATED,     /**< Joystick battery level change */

		/* Game controller events */
		CONTROLLERAXISMOTION = SDL_CONTROLLERAXISMOTION, /**< Game controller axis motion */
		CONTROLLERBUTTONDOWN = SDL_CONTROLLERBUTTONDOWN,         /**< Game controller button pressed */
		CONTROLLERBUTTONUP = SDL_CONTROLLERBUTTONUP,           /**< Game controller button released */
		CONTROLLERDEVICEADDED = SDL_CONTROLLERDEVICEADDED,        /**< A new Game controller has been inserted into the system */
		CONTROLLERDEVICEREMOVED = SDL_CONTROLLERDEVICEREMOVED,      /**< An opened Game controller has been removed */
		CONTROLLERDEVICEREMAPPED = SDL_CONTROLLERDEVICEREMAPPED,     /**< The controller mapping was updated */
		CONTROLLERTOUCHPADDOWN = SDL_CONTROLLERTOUCHPADDOWN,       /**< Game controller touchpad was touched */
		CONTROLLERTOUCHPADMOTION = SDL_CONTROLLERTOUCHPADMOTION,     /**< Game controller touchpad finger was moved */
		CONTROLLERTOUCHPADUP = SDL_CONTROLLERTOUCHPADUP,         /**< Game controller touchpad finger was lifted */
		CONTROLLERSENSORUPDATE = SDL_CONTROLLERSENSORUPDATE,       /**< Game controller sensor was updated */

		/* Touch events */
		FINGERDOWN = SDL_FINGERDOWN,
		FINGERUP = SDL_FINGERUP,
		FINGERMOTION = SDL_FINGERMOTION,

		/* Gesture events */
		DOLLARGESTURE = SDL_DOLLARGESTURE,
		DOLLARRECORD = SDL_DOLLARRECORD,
		MULTIGESTURE = SDL_MULTIGESTURE,

		/* Clipboard events */
		CLIPBOARDUPDATE = SDL_CLIPBOARDUPDATE, /**< The clipboard or primary selection changed */

		/* Drag and drop events */
		DROPFILE = SDL_DROPFILE, /**< The system requests a file open */
		DROPTEXT = SDL_DROPTEXT,          /**< text/plain drag-and-drop event */
		DROPBEGIN = SDL_DROPBEGIN,         /**< A new set of drops is beginning (NULL filename) */
		DROPCOMPLETE = SDL_DROPCOMPLETE,      /**< Current set of drops is now complete (NULL filename) */

		/* Audio hotplug events */
		AUDIODEVICEADDED = SDL_AUDIODEVICEADDED, /**< A new audio device is available */
		AUDIODEVICEREMOVED = SDL_AUDIODEVICEREMOVED,        /**< An audio device has been removed. */

		/* Sensor events */
		SENSORUPDATE = SDL_SENSORUPDATE, /**< A sensor was updated */

		/* Render events */
		RENDER_TARGETS_RESET = SDL_RENDER_TARGETS_RESET, /**< The render targets have been reset and their contents need to be updated */
		RENDER_DEVICE_RESET = SDL_RENDER_DEVICE_RESET,           /**< The device has been reset and all textures need to be recreated */

		/* Internal events */
		POLLSENTINEL = SDL_POLLSENTINEL, /**< Signals the end of an event poll cycle */

		/** Events ::SDL_USEREVENT through ::SDL_LASTEVENT are for your use,
		 *  and should be allocated with SDL_RegisterEvents()
		 */
		USEREVENT = SDL_USEREVENT,

		/**
		 *  This last event is only for bounding internal arrays
		 */
		LASTEVENT = SDL_LASTEVENT
	};
}

export
namespace sdl::win::event
{
	enum
	{
		NONE            = SDL_WINDOWEVENT_NONE,           /**< Never used */
		SHOWN           = SDL_WINDOWEVENT_SHOWN,          /**< Window has been shown */
		HIDDEN          = SDL_WINDOWEVENT_HIDDEN,         /**< Window has been hidden */
		EXPOSED         = SDL_WINDOWEVENT_EXPOSED,        /**< Window has been exposed and should be redrawn */
		MOVED           = SDL_WINDOWEVENT_MOVED,          /**< Window has been moved to data1, data2 */
		RESIZED         = SDL_WINDOWEVENT_RESIZED,        /**< Window has been resized to data1xdata2 */
		SIZE_CHANGED    = SDL_WINDOWEVENT_SIZE_CHANGED,   /**< The window size has changed, either as
																a result of an API call or through the
																system or user changing the window size. */
		MINIMIZED       = SDL_WINDOWEVENT_MINIMIZED,      /**< Window has been minimized */
		MAXIMIZED       = SDL_WINDOWEVENT_MAXIMIZED,      /**< Window has been maximized */
		RESTORED        = SDL_WINDOWEVENT_RESTORED,       /**< Window has been restored to normal size
																and position */
		ENTER           = SDL_WINDOWEVENT_ENTER,          /**< Window has gained mouse focus */
		LEAVE           = SDL_WINDOWEVENT_LEAVE,          /**< Window has lost mouse focus */
		FOCUS_GAINED    = SDL_WINDOWEVENT_FOCUS_GAINED,   /**< Window has gained keyboard focus */
		FOCUS_LOST      = SDL_WINDOWEVENT_FOCUS_LOST,     /**< Window has lost keyboard focus */
		CLOSE           = SDL_WINDOWEVENT_CLOSE,          /**< The window manager requests that the window be closed */
		TAKE_FOCUS      = SDL_WINDOWEVENT_TAKE_FOCUS,     /**< Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow, or ignore) */
		HIT_TEST        = SDL_WINDOWEVENT_HIT_TEST,       /**< Window had a hit test that wasn't SDL_HITTEST_NORMAL. */
		ICCPROF_CHANGED = SDL_WINDOWEVENT_ICCPROF_CHANGED,/**< The ICC profile of the window's display has changed. */
		DISPLAY_CHANGED = SDL_WINDOWEVENT_DISPLAY_CHANGED /**< Window has been moved to display data1. */
	};
} // namespace sdl::win::event

export
namespace sdl::renderer
{
	enum
	{
		SOFTWARE      =  SDL_RENDERER_SOFTWARE     , /**< The renderer is a software fallback */
		ACCELERATED   =  SDL_RENDERER_ACCELERATED  , /**< The renderer uses hardware acceleration */
		PRESENTVSYNC  =  SDL_RENDERER_PRESENTVSYNC , /**< Present is synchronized with the refresh rate */
		TARGETTEXTURE =  SDL_RENDERER_TARGETTEXTURE, /**< The renderer supports rendering to texture */
	};
} // namespace sdl::renderer

export

namespace img::init
{
	enum
	{
		JPG  = IMG_INIT_JPG,
		PNG  = IMG_INIT_PNG,
		TIF  = IMG_INIT_TIF,
		WEBP = IMG_INIT_WEBP,
		JXL  = IMG_INIT_JXL,
		AVIF = IMG_INIT_AVIF
	};
} // namespace img::init