using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ChangeScene : MonoBehaviour
{

    //Loads scenes based on order in build settings
    public void SceneLoader(int SceneIndex)
	{
        SceneManager.LoadScene(SceneIndex);
	}

    //Allows loading of play game scene if globalcontrol bool is true
    public void PlayGameSceneLoader()
    {
		if (GlobalControl.Instance.playGame)
		{
            SceneManager.LoadScene(2);
        }
    }

    //Quits the program
    public void Quit()
    {
        Debug.Log("Pressed the quit button!"); //System.out.print similar
        #if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;

        #else
            Application.Quit();

        #endif
    }
}
