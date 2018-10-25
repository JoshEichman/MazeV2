# -*- coding: utf-8 -*-
"""
Created on Sat Sep 29 01:05:22 2018

@author: Josh
"""

from kivy.app import App
from kivy.uix.button import Button

class TestApp(App):
    def build(self):
        return Button(text='Hello World')

TestApp().run()