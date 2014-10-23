﻿#include "pch.h"
#include "OpenGLESPage.xaml.h"

using namespace cocos2d;

App::App()
{
    InitializeComponent();
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
    if (mPage == nullptr)
    {
        mPage = ref new cocos2d::OpenGLESPage(&mOpenGLES);
    }

    // Place the page in the current window and ensure that it is active.
    Windows::UI::Xaml::Window::Current->Content = mPage;
    Windows::UI::Xaml::Window::Current->Activate();
}
