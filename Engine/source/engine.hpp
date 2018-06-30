#pragma once

#include "precompiled.hpp"
#include "GameTimer.hpp"
#include "DirectXRenderer.hpp"
#include "WindowsMessageHandlingService.hpp"
#include "CommandQueue.hpp"
#include "InputHandler.hpp"



namespace mt
{
	class Scene;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A mt engine. </summary>
	/// <detail>	The engine will handle processing of low level input (pass to input handler)
	/// 			the engine will handle all messages from windows
	/// 			that includes managing resizes
	/// <remarks>	Micho, 6/13/2016. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class engine
	{
	protected:
		static std::unique_ptr<engine> _instance;
	
		std::thread _engine_tick_thread;
	
	public:	
		// ACCESSOR
	
		static	engine& 					    get_engine()					{ return *engine::_instance.get(); };
		static	GameTimer&						get_game_timer()				{ return get_engine().time; };
		static	DirectXRenderer&				get_renderer()					{ return get_engine().dxr; };
		static  InputHandler&					get_input_handler()				{ return get_engine().input_handler; }
		static  Camera&							get_current_camera()			{ return get_engine().dxr.get_current_camera(); }
		static	WindowsMessageHandlingService&	get_windows_message_handler()	{ return get_engine().message_handling_service; };
		static	CommandQueue&					get_command_manager()			{ return get_engine().command_manager; }
		static	HINSTANCE						get_app_inst()					{ return get_engine()._get_app_inst(); };
		static	HWND							get_main_window_handle()		{ return get_engine()._get_main_window_handle(); };
		static	bool							get_is_maximized()				{ return get_engine()._get_is_maximized(); };
		static	bool							get_is_minimized()				{ return get_engine()._get_is_minimized(); };
		static	bool							get_is_resizing()				{ return get_engine()._get_is_resizing(); };
		static	bool							get_is_fullscreen()				{ return get_engine()._get_is_fullscreen(); };
		static	float							get_aspect_ratio()				{ return get_engine()._get_aspect_ratio(); };
		static	int								get_width()						{ return get_engine()._get_width(); };
		static	int								get_height()					{ return get_engine()._get_height(); };
	
		// MUTATOR
	
		static Status run() 
		{ 
			return (_instance.get() == nullptr) ? Status::failure : get_engine()._run(); 
		};
		
		static bool initialize(HINSTANCE hInstance)
		{	
			return get_engine()._initialize(hInstance);;
		};
		
		static void resize(int width, int height)
		{ 
			return get_engine()._resize(width, height); 
		};
	
		static void set_is_resizing(bool is_resizing)
		{
			return get_engine()._set_is_resizing(is_resizing);
		}
	
		static void set_is_maximized(bool is_maximized)
		{
			return get_engine()._set_is_maximized(is_maximized);
		}
		
		static void set_is_minimized(bool is_resizing)
		{
			return get_engine()._set_is_minimized(is_resizing);
		}
	
		static void set_window_dimensions(const int& width, const int& height)
		{
			get_engine()._client_width = width;
			get_engine()._client_height = height;
			get_engine()._client_aspect_ratio = (float)width / (float)height;
		}
	
		static void shutdown()
		{
			get_engine()._shutdown();
			OutputDebugStringW(L"Engine Shutdown Initiated\n");
		}
	
		static void destroy()
		{
			OutputDebugStringW(L"Engine Destroyed\n");
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <remarks>	Micho, 6/13/2016. </remarks>
		///
		/// <param name="hInstance">	The instance. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		engine() = default;
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Destructor. </summary>
		///
		/// <remarks>	Micho, 6/13/2016. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual ~engine() = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor. </summary>
		///
		/// <remarks>	Micho, 6/13/2016. </remarks>
		///
		/// <param name="other">	The other. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		engine(const engine&  other) = delete;
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Move constructor. </summary>
		///
		/// <remarks>	Micho, 6/13/2016. </remarks>
		///
		/// <param name="other">	[in,out] The other. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		engine(engine&& other) = delete;
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator. </summary>
		///
		/// <remarks>	Micho, 6/13/2016. </remarks>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	A shallow copy of this object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		engine& operator=(const engine&  other) = delete;
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Move assignment operator. </summary>
		///
		/// <remarks>	Micho, 6/13/2016. </remarks>
		///
		/// <param name="other">	[in,out] The other. </param>
		///
		/// <returns>	A shallow copy of this object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		engine& operator=(engine&& other) = delete;
	
	protected:	
		// ACCESSORS
	
		HINSTANCE _get_app_inst()			const	{ return _application_instance; };
		HWND      _get_main_window_handle() const	{ return _main_window_handle; };
		bool	  _get_is_maximized()		const	{ return _is_maximized; };
		bool	  _get_is_minimized()		const	{ return _is_minimized; };
		bool	  _get_is_resizing()		const	{ return _is_resizing; };
		bool	  _get_is_fullscreen()		const	{ return _is_fullscreen; };
		float	  _get_aspect_ratio()		const	{ return _client_aspect_ratio; };
		int		  _get_width()				const	{ return _client_width; };
		int		  _get_height()				const	{ return _client_height; };
	
		virtual void OnResize() { };
		virtual void Update() { };
		virtual void Draw() { };
	
		virtual bool _initialize(HINSTANCE hInstance);
		Status	 _run();
		void	_tick();
		
		bool _init_main_window();
		void _update_frame_stats();
	
	protected:
	
		static void tick();
	
		void _resize(int width, int height)
		{
			// This flag should prevent futher rendering after the current frame finishes
			_is_resizing = true;
	
			// wait until rendering is finished.
			while (dxr.get_is_rendering()) {};
	
			_client_width = width;
			_client_height = height;
			_client_aspect_ratio = static_cast<float>(_client_width) / _client_height;
	
			dxr.resize(width, height);
			
			// Trigger callback
			OnResize();
			// Continue rendering.
			_is_resizing = false;
		}
	
		void _set_is_resizing(bool is_resizing)
		{
			_is_resizing = is_resizing;
		}
	
		void _set_is_maximized(bool is_maximized)
		{
			_is_maximized = is_maximized;
		}
	
		void _set_is_minimized(bool is_minimized)
		{
			_is_minimized = is_minimized;
		}
	
		void _shutdown()
		{
			_is_shutdown = true;
		}
	
	private:
		DirectXRenderer					dxr;
		WindowsMessageHandlingService	message_handling_service;
		CommandQueue					command_manager;
		InputHandler					input_handler;
		std::wstring					main_window_caption = L"mt_engine";
	
		int		_client_width			= 0;
		int		_client_height			= 0;
		float	_client_aspect_ratio	= _client_height ? static_cast<float>(_client_width) / _client_height : 0;
	
		std::chrono::nanoseconds time_since_stat_update = 0ns;
	
		HINSTANCE					_application_instance	= nullptr;	// application instance handle
		HWND						_main_window_handle		= nullptr;	// main window handle
		volatile bool				_is_minimized			= false;	// is the application minimized?
		volatile bool				_is_maximized			= false;	// is the application maximized?
		volatile bool				_is_resizing			= false;	// are the resize bars being dragged?
		volatile bool				_is_fullscreen			= false;	// fullscreen enabled
		volatile bool				_is_shutdown			= false;	// shutdown

		GameTimer time;
	
		Scene*	_default_scene;
	};
}

